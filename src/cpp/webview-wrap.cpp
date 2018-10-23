#include "webview-wrap.h"
#include "js-types.h"

using Napi::Object;
using Napi::Array;
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

  if (info.Length() > 0) {
    expectJSType(info[0], object);
    Object arg = info[0].As<Object>();
    Object self = info.This().As<Object>();

    Function assign = info.Env().Global().Get("Object").As<Object>().Get("assign").As<Function>();
    assign.Call({ self, arg });
  }
}

Webview::~Webview() {
  free(web);
  web = nullptr;
}

Object Webview::Init(Napi::Env env, Object exports) {
  Napi::HandleScope scope(env);

  Function func = DefineClass(env, "WebView", {
    InstanceAccessor("url", &Webview::GetURL, &Webview::SetURL),
    InstanceAccessor("title", &Webview::GetTitle, &Webview::SetTitle),
    InstanceAccessor("width", &Webview::GetWidth, &Webview::SetWidth),
    InstanceAccessor("height", &Webview::GetHeight, &Webview::SetHeight),
    InstanceAccessor("resizable", &Webview::GetResizable, &Webview::SetResizable),
    InstanceAccessor("debug", &Webview::GetDebug, &Webview::SetDebug),

    InstanceMethod("init", &Webview::Init),
    InstanceMethod("loop", &Webview::Loop),
    InstanceMethod("dialog", &Webview::Dialog),
    InstanceMethod("terminate", &Webview::Terminate),
    InstanceMethod("eval", &Webview::Eval),
    InstanceMethod("injectCSS", &Webview::InjectCSS),
    InstanceMethod("exit", &Webview::Exit),
    InstanceMethod("setFullscreen", &Webview::SetFullScreen),
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("WebView", func);
  return exports;
}

void Webview::SetExternalInvokeCB(const CallbackInfo& info, const Napi::Value& value) {
  expectJSType(value, function);
  external_invoke = value.As<Function>();
}

void Webview::ExternalInvoke(std::string arg) {
  external_invoke.Call(0, { String::New(external_invoke.Env(), arg) });
}

// URL
Value Webview::GetURL(const CallbackInfo& info) {
  return String::New(info.Env(), web->url);
}
void Webview::SetURL(const CallbackInfo& info, const Napi::Value& value) {
  expectJSType(value, string);
  web->url = toCString(value.As<String>());
}

// Title
Value Webview::GetTitle(const CallbackInfo& info) {
  return String::New(info.Env(), web->title);
}
void Webview::SetTitle(const CallbackInfo& info, const Napi::Value& value) {
  expectJSType(value, string);
  const char* title = toCString(value.As<String>());
  if (loaded) {
    webview_set_title(web, title);
  } else {
    web->title = title;
  }
}

// Width
Value Webview::GetWidth(const CallbackInfo& info) {
  return Number::New(info.Env(), web->width);
}
void Webview::SetWidth(const CallbackInfo& info, const Napi::Value& value) {
  expectJSType(value, number);
  double width = value.As<Number>();
  web->width = width;
}

// Height
Value Webview::GetHeight(const CallbackInfo& info) {
  return Number::New(info.Env(), web->height);
}
void Webview::SetHeight(const CallbackInfo& info, const Napi::Value& value) {
  expectJSType(value, number);
  web->height = value.As<Number>();
}

// Resizable
Value Webview::GetResizable(const CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), web->resizable);
}
void Webview::SetResizable(const CallbackInfo& info, const Napi::Value& value) {
  expectJSType(value, boolean);
  web->resizable = value.As<Napi::Boolean>();
}

// Debug
Value Webview::GetDebug(const CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), web->debug);
}
void Webview::SetDebug(const CallbackInfo& info, const Napi::Value& value) {
  expectJSType(value, boolean);
  web->debug = value.As<Napi::Boolean>();
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

Value Webview::SetFullScreen(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], boolean);
  bool fullscreen = info[0].ToBoolean();
  webview_set_fullscreen(web, fullscreen);
  return info.This();
}

// Methods


Value Webview::Init(const CallbackInfo& info) {
  loaded = true;
  webview_init(web);
  return info.Env().Undefined();
}

Value Webview::Loop(const CallbackInfo& info) {
  if (!loaded) {
    throw Napi::Error::New(info.Env(), "Webview can only enter loop when loaded.");
  }
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
