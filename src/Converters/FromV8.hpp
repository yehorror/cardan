#pragma once

#include <v8.h>

namespace cardan::FromV8
{
    template <class Type>
    Type convert(v8::Local<v8::Context> context, v8::Local<v8::Value> value);
}
