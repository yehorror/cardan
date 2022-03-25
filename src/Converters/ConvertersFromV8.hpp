#pragma once

#include <v8.h>

namespace cardan::converters
{
    template <class Type>
    Type convertArgumentFromV8Value(v8::Local<v8::Context> context, v8::Local<v8::Value> value);
}
