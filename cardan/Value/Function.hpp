#pragma once

#include <vector>
#include <string>

#include "Value.hpp"
#include "cardan/Converters/ToV8.hpp"
#include "cardan/Helper.hpp"

namespace cardan
{
    class Value;

    // TODO This whole class consists on 90% of template functions
    // Consider removal of cpp file (it has only one simple method + constructr) and move it here or .inl file
    class Function
    {
    public:
        Function(v8::Local<v8::Function> function, Context& context);

        // TODO Move to inl file
        template<class...Args>
        Value call(Args... args);

    public:
        v8::Local<v8::Function> m_function;
        Context& m_context;
    };
}

#include "Function.inl"
