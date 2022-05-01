#pragma once

#include "Constructor.hpp"

namespace cardan::classDetails
{
    template <class ClassT, typename... Args>
    class ConstructorWithBindings : public Constructor
    {
    public:
        ConstructorWithBindings(Args&&... args);
        void addConstructor(Context& context, v8::Local<v8::FunctionTemplate>& constructorFuncTemplate) override;

    private:
        std::tuple<Args...> m_args;
    };
}

#include "ConstructorWithBindings.inl"
