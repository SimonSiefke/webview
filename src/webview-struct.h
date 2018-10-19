#ifndef WEBVIEW_STRUCT_H
#define WEBVIEW_STRUCT_H

#include "webview.h"
#include <stdlib.h>
#include <napi.h>

class WebviewStruct: public Napi::ObjectWrap<WebviewStruct> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static WebviewStruct* fromArgs(const Napi::CallbackInfo& info);

  WebviewStruct(const Napi::CallbackInfo& info);
  ~WebviewStruct();

  Napi::Value GetURL(const Napi::CallbackInfo& info);
  Napi::Value GetTitle(const Napi::CallbackInfo& info);
  Napi::Value GetWidth(const Napi::CallbackInfo& info);
  Napi::Value GetHeight(const Napi::CallbackInfo& info);
  Napi::Value GetResizable(const Napi::CallbackInfo& info);
  Napi::Value GetDebug(const Napi::CallbackInfo& info);

  Napi::Value SetURL(const Napi::CallbackInfo& info);
  Napi::Value SetTitle(const Napi::CallbackInfo& info);
  Napi::Value SetWidth(const Napi::CallbackInfo& info);
  Napi::Value SetHeight(const Napi::CallbackInfo& info);
  Napi::Value SetResizable(const Napi::CallbackInfo& info);
  Napi::Value SetDebug(const Napi::CallbackInfo& info);
  Napi::Value SetExternalInvokeCB(const Napi::CallbackInfo& info);
  struct webview *getStruct();


private:
  static Napi::FunctionReference constructor;
  void ExternalInvoke(std::string arg);

  Napi::Function external_invoke;
  struct webview *web = nullptr;

  void *userdata;
};

#endif
