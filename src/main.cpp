#define WEBVIEW_IMPLEMENTATION

#include <napi.h>
#include "js-types.h"
#include "webview-struct.h"

using Napi::Value;
using Napi::Function;
using Napi::Object;
using Napi::String;

Value WebViewInit(const Napi::CallbackInfo& info) {
  expectArgumentSize(info, 1);
  struct webview *w = WebviewStruct::fromArgs(info)->getStruct();
  webview_init(w);
  while (webview_loop(w, false) == 0);
  return info.Env().Undefined();
}

Value WebViewSetTitle(const Napi::CallbackInfo& info) {
  expectArgumentSize(info, 2);
  expectJSType(info[1], string);

  webview_set_title(WebviewStruct::fromArgs(info)->getStruct(), toCString(info[1].ToString()));
  return info.Env().Undefined();
}

Value WebViewTerminate(const Napi::CallbackInfo& info) {
  expectArgumentSize(info, 1);
  webview_terminate(WebviewStruct::fromArgs(info)->getStruct());
  return info.Env().Undefined();
}

Value WebViewEval(const Napi::CallbackInfo& info) {
  expectArgumentSize(info, 2);
  expectJSType(info[1], string);

  webview_eval(WebviewStruct::fromArgs(info)->getStruct(), toCString(info[1].ToString()));
  return info.Env().Undefined();
}

Value WebViewInjectCSS(const Napi::CallbackInfo& info) {
  expectArgumentSize(info, 2);
  expectJSType(info[1], string);
  webview_inject_css(WebviewStruct::fromArgs(info)->getStruct(), toCString(info[1].ToString()));
  return info.Env().Undefined();
}

/*
Value WebviewDialog(
  struct webview *w,
  enum webview_dialog_type dlgtype,
  int flags,
  const char *title,
  const char *arg,
  char *result,
  size_t resultsz
) {
}
*/

Value WebViewExit(const Napi::CallbackInfo& info) {
  expectArgumentSize(info, 1);
  webview_exit(WebviewStruct::fromArgs(info)->getStruct());
  return info.Env().Undefined();
}

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
  WebviewStruct::Init(env, exports);
  exports.Set(String::New(env, "init"), Function::New(env, WebViewInit));
  exports.Set(String::New(env, "setTitle"), Function::New(env, WebViewSetTitle));
  exports.Set(String::New(env, "terminate"), Function::New(env, WebViewTerminate));
  exports.Set(String::New(env, "eval"), Function::New(env, WebViewEval));
  exports.Set(String::New(env, "injectCSS"), Function::New(env, WebViewInjectCSS));
  exports.Set(String::New(env, "exit"), Function::New(env, WebViewExit));
  exports.Set(String::New(env, "debug"), Function::New(env, WebViewDebug));
  exports.Set(String::New(env, "printLog"), Function::New(env, WebViewPrintLog));

  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)
