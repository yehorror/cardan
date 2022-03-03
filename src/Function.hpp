#pragma once

#include "Value.hpp"

namespace cardan
{
    class Function
    {
    public:

        template<class...Args>
        Value call(Args... args)
        {
            //return m_function->Call();
        }

    private:
        friend class Value;
        explicit Function(v8::Local<v8::Function> function);

    public:
        v8::Local<v8::Function> m_function;
    };
}
