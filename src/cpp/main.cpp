#define WEBVIEW_IMPLEMENTATION

#include <napi.h>
#include "js-types.h"
#include "webview-wrap.h"

using Napi::Value;
using Napi::Function;
using Napi::Object;
using Napi::String;

Value WebViewDebug(const Napi::CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], string);
  webview_debug(toCString(info[0].ToString()));
  return info.Env().Undefined();
}

Value WebViewPrintLog(const Napi::CallbackInfo& info) {
  expectArgumentSize(info, 1);
  expectJSType(info[0], string);
  webview_print_log(toCString(info[0].ToString()));
  return info.Env().Undefined();
}

Object Init(Napi::Env env, Object exports) {
  Webview::Init(env, exports);
  exports.Set(String::New(env, "debug"), Function::New(env, WebViewDebug));
  exports.Set(String::New(env, "printLog"), Function::New(env, WebViewPrintLog));

  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
