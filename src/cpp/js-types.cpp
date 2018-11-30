#include <stdlib.h>
#include "js-types.h"

const char* toCString(Napi::String v8_string) {
  std::string str = v8_string.Utf8Value();
  // Temporary variable to prevent garbage collection
  char* res = new char[str.length() + 1];
  std::strcpy(res, str.c_str());
  return res;
}

void expectString(const Napi::Value value) {
  if (!value.IsString()) {
    throw Napi::TypeError::New(value.Env(), "Expected a string.");
  }
}
void expectNumber(const Napi::Value value) {
  if (!value.IsNumber()) {
    throw Napi::TypeError::New(value.Env(), "Expected a number.");
  }
}
void expectFunction(const Napi::Value value) {
  if (!value.IsFunction()) {
    throw Napi::TypeError::New(value.Env(), "Expected a function.");
  }
}
void expectBoolean(const Napi::Value value) {
  if (!value.IsBoolean()) {
    throw Napi::TypeError::New(value.Env(), "Expected a boolean.");
  }
}
void expectObject(const Napi::Value value) {
  if (!value.IsObject()) {
    throw Napi::TypeError::New(value.Env(), "Expected a object.");
  }
}

void expectJSType(const Napi::Value value, JSType type) {
  switch (type) {
    case string:   return expectString(value);
    case number:   return expectNumber(value);
    case function: return expectFunction(value);
    case boolean:  return expectBoolean(value);
    case object:   return expectObject(value);
  }
}

void expectArgumentSize(const Napi::CallbackInfo& info, int size) {
  if ((int) info.Length() < size) {
    throw Napi::Error::New(
      info.Env(),
      "Expected at least " + std::to_string(size)
        + " argument" + (size == 1 ? "." : "s.")
    );
  }
}
