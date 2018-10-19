#ifndef JS_TYPES_H
#define JS_TYPES_H
#include <napi.h>

enum JSType {
  string, number, function, boolean
};

const char* toCString(Napi::String v8_string);
void expectJSType(const Napi::Value value, JSType type);
void expectArgumentSize(const Napi::CallbackInfo& info, int size);

#endif
