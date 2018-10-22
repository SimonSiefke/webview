#include "webview-wrap.h"
#include "js-types.h"

using Napi::Object;
using Napi::Function;
using Napi::Value;
using Napi::Number;
using Napi::String;
using Napi::CallbackInfo;

Napi::FunctionReference Webview::constructor;

Webview::Webview(const CallbackInfo& info) : Napi::ObjectWrap<Webview>(info) {
  web = (struct webview *) malloc(sizeof(struct webview));
  web->title = "";
  web->url = "";
  web->height = 100;
  web->width = 100;
  web->debug = false;
  web->resizable = true;
}

Webview::~Webview() {
  free(web);
  web = nullptr;
}

Object Webview::Init(Napi::Env env, Object exports) {
  Napi::HandleScope scope(env);

  Function func = DefineClass(env, "WebView", {
    InstanceMethod("getURL", &Webview::GetURL),
    InstanceMethod("setURL", &Webview::SetURL),
    InstanceMethod("getTitle", &Webview::GetTitle),
    InstanceMethod("setTitle", &Webview::SetTitle),
    InstanceMethod("getWidth", &Webview::GetWidth),
    InstanceMethod("setWidth", &Webview::SetWidth),
    InstanceMethod("getHeight", &Webview::GetHeight),
    InstanceMethod("setHeight", &Webview::SetHeight),
    InstanceMethod("getResizable", &Webview::GetResizable),
    InstanceMethod("setResizable", &Webview::SetResizable),
    InstanceMethod("getDebug", &Webview::GetDebug),
    InstanceMethod("setDebug", &Webview::SetDebug),

    InstanceMethod("init", &Webview::Init),
    InstanceMethod("loop", &Webview::Loop),
    InstanceMethod("dialog", &Webview::Dialog),
    InstanceMethod("terminate", &Webview::Terminate),
    InstanceMethod("eval", &Webview::Eval),
    InstanceMethod("injectCSS", &Webview::InjectCSS),
    InstanceMethod("exit", &Webview::Exit)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("WebView", func);
  return exports;
}

Value Webview::SetExternalInvokeCB(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], function);
  external_invoke = info[0].As<Function>();
  return info.Env().Undefined();
}

void Webview::ExternalInvoke(std::string arg) {
  external_invoke.Call(0, { String::New(external_invoke.Env(), arg) });
}

// URL
Value Webview::GetURL(const CallbackInfo& info) {
  return String::New(info.Env(), web->url);
}
Value Webview::SetURL(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], string);
  web->url = toCString(info[0].As<String>());
  return GetURL(info);
}

// Title
Value Webview::GetTitle(const CallbackInfo& info) {
  return String::New(info.Env(), web->title);
}
Value Webview::SetTitle(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], string);
  const char* title = toCString(info[0].As<String>());
  if (loaded) {
    webview_set_title(web, title);
  } else {
    web->title = title;
  }
  return GetTitle(info);
}

// Width
Value Webview::GetWidth(const CallbackInfo& info) {
  return Number::New(info.Env(), web->width);
}
Value Webview::SetWidth(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], number);
  double width = info[0].As<Number>();
  web->width = width;
  return GetWidth(info);
}

// Height
Value Webview::GetHeight(const CallbackInfo& info) {
  return Number::New(info.Env(), web->height);
}
Value Webview::SetHeight(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], number);
  web->height = info[0].As<Number>();
  return GetHeight(info);
}

// Resizable
Value Webview::GetResizable(const CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), web->resizable);
}
Value Webview::SetResizable(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], boolean);
  web->resizable = info[0].As<Napi::Boolean>();
  return GetResizable(info);
}

// Debug
Value Webview::GetDebug(const CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), web->debug);
}
Value Webview::SetDebug(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], boolean);
  web->debug = info[0].As<Napi::Boolean>();
  return GetDebug(info);
}

// Dialog

/**
 * @param dlgtype
 * enum webview_dialog_type {
 *   WEBVIEW_DIALOG_TYPE_OPEN = 0,
 *   WEBVIEW_DIALOG_TYPE_SAVE = 1,
 *   WEBVIEW_DIALOG_TYPE_ALERT = 2
 * };
 */
Value Webview::Dialog(const CallbackInfo& info) {
  expectArgumentSize(info, 4);
  expectJSType(info[0], number);
  expectJSType(info[1], number);
  expectJSType(info[2], string);
  expectJSType(info[3], string);

  int dlgtype = info[0].As<Number>();
  int flags = info[1].As<Number>();
  const char* title = toCString(info[2].As<String>());
  const char* arg = toCString(info[3].As<String>());

  if (flags < 0 || flags > 2) {
    throw Napi::TypeError::New(info.Env(), "Invalid flag for Webview.Dialog");
  }

  char result[4096] = "";
  webview_dialog(web, static_cast<webview_dialog_type>(dlgtype), flags, title, arg, result, sizeof(result));

  return String::New(info.Env(), result);
}


// Methods
Value Webview::Init(const CallbackInfo& info) {
  loaded = true;
  webview_init(web);
  // while (webview_loop(web, false) == 0);
  return info.Env().Undefined();
}

Value Webview::Loop(const CallbackInfo& info) {
  if (!loaded) {
    throw Napi::Error::New(info.Env(), "Webview can only enter loop when loaded.");
  }
  loaded = true;
  return Number::New(info.Env(), webview_loop(web, true));
}

Value Webview::Terminate(const CallbackInfo& info) {
  loaded = false;
  webview_terminate(web);
  return info.Env().Undefined();
}

Value Webview::Eval(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], string);
  webview_eval(web, toCString(info[0].As<String>()));
  return info.Env().Undefined();
}

Value Webview::InjectCSS(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], string);
  webview_inject_css(web, toCString(info[0].As<String>()));
  return info.Env().Undefined();
}

Value Webview::Exit(const CallbackInfo& info) {
  loaded = false;
  webview_exit(web);
  return info.Env().Undefined();
}
