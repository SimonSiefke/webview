#include <stdlib.h>
#include "webview-struct.h"
#include "js-types.h"

using Napi::Object;
using Napi::Function;
using Napi::Value;
using Napi::Number;
using Napi::String;
using Napi::Boolean;
using Napi::CallbackInfo;

Napi::FunctionReference WebviewStruct::constructor;

WebviewStruct::WebviewStruct(const CallbackInfo& info) : Napi::ObjectWrap<WebviewStruct>(info) {
  web = (struct webview *) malloc(sizeof(struct webview));
  web->title = "";
  web->url = "";
  web->height = 100;
  web->width = 100;
  web->debug = false;
  web->resizable = true;
}

WebviewStruct::~WebviewStruct() {
  free(web);
  web = nullptr;
}

WebviewStruct* WebviewStruct::fromArgs(const CallbackInfo& info) {
  WebviewStruct* obj;
  napi_status status = napi_unwrap(info.Env(), info[0], reinterpret_cast<void**>(&obj));
  if (status != napi_ok) {
    throw Napi::TypeError::New(info.Env(), "Cannot cast to WebView object.");
  }
  return obj;
}

struct webview* WebviewStruct::getStruct() {
  return web;
}

Object WebviewStruct::Init(Napi::Env env, Object exports) {
  Napi::HandleScope scope(env);

  Function func = DefineClass(env, "WebView", {
    InstanceMethod("getURL", &WebviewStruct::GetURL),
    InstanceMethod("setURL", &WebviewStruct::SetURL),
    InstanceMethod("getTitle", &WebviewStruct::GetTitle),
    InstanceMethod("setTitle", &WebviewStruct::SetTitle),
    InstanceMethod("getWidth", &WebviewStruct::GetWidth),
    InstanceMethod("setWidth", &WebviewStruct::SetWidth),
    InstanceMethod("getHeight", &WebviewStruct::GetHeight),
    InstanceMethod("setHeight", &WebviewStruct::SetHeight),
    InstanceMethod("getResizable", &WebviewStruct::GetResizable),
    InstanceMethod("setResizable", &WebviewStruct::SetResizable),
    InstanceMethod("getDebug", &WebviewStruct::GetDebug),
    InstanceMethod("setDebug", &WebviewStruct::SetDebug),

    InstanceMethod("init", &WebviewStruct::Init),
    InstanceMethod("loop", &WebviewStruct::Loop),
    InstanceMethod("dialog", &WebviewStruct::Dialog),
    InstanceMethod("terminate", &WebviewStruct::Terminate),
    InstanceMethod("eval", &WebviewStruct::Eval),
    InstanceMethod("injectCSS", &WebviewStruct::InjectCSS),
    InstanceMethod("exit", &WebviewStruct::Exit)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("WebView", func);
  return exports;
}

Value WebviewStruct::SetExternalInvokeCB(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], function);
  external_invoke = info[0].As<Function>();
  return info.Env().Undefined();
}

void WebviewStruct::ExternalInvoke(std::string arg) {
  external_invoke.Call(0, { Napi::String::New(external_invoke.Env(), arg) });
}

// URL
Value WebviewStruct::GetURL(const CallbackInfo& info) {
  return Napi::String::New(info.Env(), web->url);
}
Value WebviewStruct::SetURL(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], string);
  web->url = toCString(info[0].As<Napi::String>());
  return GetURL(info);
}

// Title
Value WebviewStruct::GetTitle(const CallbackInfo& info) {
  return Napi::String::New(info.Env(), web->title);
}
Value WebviewStruct::SetTitle(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], string);
  const char* title = toCString(info[0].As<Napi::String>());
  if (loaded) {
    webview_set_title(web, title);
  } else {
    web->title = title;
  }
  return GetTitle(info);
}

// Width
Value WebviewStruct::GetWidth(const CallbackInfo& info) {
  return Number::New(info.Env(), web->width);
}
Value WebviewStruct::SetWidth(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], number);
  double width = info[0].As<Number>();
  web->width = width;
  return GetWidth(info);
}

// Height
Value WebviewStruct::GetHeight(const Napi::CallbackInfo& info) {
  return Number::New(info.Env(), web->height);
}
Value WebviewStruct::SetHeight(const Napi::CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], number);
  web->height = info[0].As<Number>();
  return GetHeight(info);
}

// Resizable
Value WebviewStruct::GetResizable(const CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), web->resizable);
}
Value WebviewStruct::SetResizable(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], boolean);
  web->resizable = info[0].As<Napi::Boolean>();
  return GetResizable(info);
}

// Debug
Value WebviewStruct::GetDebug(const CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), web->debug);
}
Value WebviewStruct::SetDebug(const CallbackInfo& info) {
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
Value WebviewStruct::Dialog(const CallbackInfo& info) {
  expectArgumentSize(info, 4);
  expectJSType(info[0], number);
  expectJSType(info[1], number);
  expectJSType(info[2], string);
  expectJSType(info[3], string);

  int dlgtype = info[0].As<Number>();
  int flags = info[1].As<Number>();
  const char* title = toCString(info[2].As<String>());
  const char* arg = toCString(info[3].As<String>());

  char result[4096] = "";
  webview_dialog(web, static_cast<webview_dialog_type>(dlgtype), flags, title, arg, result, sizeof(result));

  return String::New(info.Env(), result);
}


// Methods
Value WebviewStruct::Init(const CallbackInfo& info) {
  loaded = true;
  webview_init(web);
  // while (webview_loop(web, false) == 0);
  return info.Env().Undefined();
}

Value WebviewStruct::Loop(const CallbackInfo& info) {
  loaded = true;
  return Number::New(info.Env(), webview_loop(web, true));
}

Value WebviewStruct::Terminate(const CallbackInfo& info) {
  loaded = false;
  webview_terminate(web);
  return info.Env().Undefined();
}

Value WebviewStruct::Eval(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], string);
  webview_eval(web, toCString(info[0].As<String>()));
  return info.Env().Undefined();
}

Value WebviewStruct::InjectCSS(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], string);
  webview_inject_css(web, toCString(info[0].As<String>()));
  return info.Env().Undefined();
}

Value WebviewStruct::Exit(const CallbackInfo& info) {
  loaded = false;
  webview_exit(web);
  return info.Env().Undefined();
}
