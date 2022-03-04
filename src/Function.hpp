#pragma once

#include "Value.hpp"

namespace cardan
{
    class Function;
    class Value;

    class Function
    {
    public:

        //template<class...Args>
        Value call(/*Args... args*/);

    private:
        friend class Value;
        Function(v8::Local<v8::Function> function, v8::Isolate* isolate, v8::Local<v8::Context>& context);

    public:
        v8::Local<v8::Function> m_function;
        v8::Isolate* m_isolate;
        v8::Local<v8::Context>& m_context;
    };
}
