#include <napi.h>
#include "webview.h"

Napi::String Method(const Napi::CallbackInfo& info) {
  webview::webview w(true, nullptr);
  w.set_title("Minimal example");
  w.set_size(480, 320, WEBVIEW_HINT_NONE);
  w.navigate("http://example.com");
  w.run();
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "createWebview"),
              Napi::Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(hello, Init)
