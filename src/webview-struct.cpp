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
  return this->web;
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
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("WebView", func);
  return exports;
}

Value WebviewStruct::SetExternalInvokeCB(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], function);
  this->external_invoke = info[0].As<Function>();
  return info.Env().Undefined();
}

void WebviewStruct::ExternalInvoke(std::string arg) {
  this->external_invoke.Call(0, { Napi::String::New(this->external_invoke.Env(), arg) });
}

// URL
Value WebviewStruct::GetURL(const CallbackInfo& info) {
  return Napi::String::New(info.Env(), this->web->url);
}
Value WebviewStruct::SetURL(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], string);
  this->web->url = toCString(info[0].As<Napi::String>());
  return this->GetURL(info);
}

// Title
Value WebviewStruct::GetTitle(const CallbackInfo& info) {
  return Napi::String::New(info.Env(), this->web->title);
}
Value WebviewStruct::SetTitle(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], string);
  this->web->title = toCString(info[0].As<Napi::String>());
  return this->GetTitle(info);
}

// Width
Value WebviewStruct::GetWidth(const CallbackInfo& info) {
  return Number::New(info.Env(), this->web->width);
}
Value WebviewStruct::SetWidth(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], number);
  double width = info[0].As<Number>();
  this->web->width = width;
  return this->GetWidth(info);
}

// Height
Value WebviewStruct::GetHeight(const Napi::CallbackInfo& info) {
  return Number::New(info.Env(), this->web->height);
}
Value WebviewStruct::SetHeight(const Napi::CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], number);
  this->web->height = info[0].As<Number>();
  return this->GetHeight(info);
}

// Resizable
Value WebviewStruct::GetResizable(const CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), this->web->resizable);
}
Value WebviewStruct::SetResizable(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], boolean);
  this->web->resizable = info[0].As<Napi::Boolean>();
  return this->GetResizable(info);
}

// Debug
Value WebviewStruct::GetDebug(const CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), this->web->debug);
}
Value WebviewStruct::SetDebug(const CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], boolean);
  this->web->debug = info[0].As<Napi::Boolean>();
  return this->GetDebug(info);
}
