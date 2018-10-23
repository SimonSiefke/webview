#ifndef WEBVIEW_STRUCT_H
#define WEBVIEW_STRUCT_H

#include "webview.h"
#include <napi.h>

class Webview: public Napi::ObjectWrap<Webview> {
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);

  Webview(const Napi::CallbackInfo& info);
  ~Webview();

  Napi::Value Dialog(const Napi::CallbackInfo& info);
  Napi::Value Init(const Napi::CallbackInfo& info);
  Napi::Value Loop(const Napi::CallbackInfo& info);
  Napi::Value Terminate(const Napi::CallbackInfo& info);
  Napi::Value Eval(const Napi::CallbackInfo& info);
  Napi::Value InjectCSS(const Napi::CallbackInfo& info);
  Napi::Value Exit(const Napi::CallbackInfo& info);

  Napi::Value SetFullScreen(const Napi::CallbackInfo& info);

private:
  static Napi::FunctionReference constructor;
  void ExternalInvoke(std::string arg);

  Napi::Value GetURL(const Napi::CallbackInfo& info);
  Napi::Value GetTitle(const Napi::CallbackInfo& info);
  Napi::Value GetWidth(const Napi::CallbackInfo& info);
  Napi::Value GetHeight(const Napi::CallbackInfo& info);
  Napi::Value GetResizable(const Napi::CallbackInfo& info);
  Napi::Value GetDebug(const Napi::CallbackInfo& info);

  void SetURL(const Napi::CallbackInfo& info, const Napi::Value& value);
  void SetTitle(const Napi::CallbackInfo& info, const Napi::Value& value);
  void SetWidth(const Napi::CallbackInfo& info, const Napi::Value& value);
  void SetHeight(const Napi::CallbackInfo& info, const Napi::Value& value);
  void SetResizable(const Napi::CallbackInfo& info, const Napi::Value& value);
  void SetDebug(const Napi::CallbackInfo& info, const Napi::Value& value);
  void SetExternalInvokeCB(const Napi::CallbackInfo& info, const Napi::Value& value);

  Napi::Function external_invoke;
  bool loaded = false;
  struct webview *web = nullptr;

  void *userdata;
};

#endif
