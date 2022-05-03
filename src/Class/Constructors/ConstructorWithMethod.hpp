#pragma once

#include <functional>
#include "Constructor.hpp"

namespace cardan::classDetails
{
    template <class ClassT, class MethodT>
    class ConstructorWithMethod : public Constructor
    {
    public:
        ConstructorWithMethod(MethodT&& method);
        void addConstructor(Context& context, v8::Local<v8::FunctionTemplate>& constructorFuncTemplate) override;

    private:
        template <typename... Args>
        void addConstructorInternal(
            Context& context,
            v8::Local<v8::FunctionTemplate>& constructorFuncTemplate,
            std::function<ClassT*(Args...)> constructFunction
        );

        template <typename... Args>
        static void v8Constructor(const v8::FunctionCallbackInfo<v8::Value>& callInfo);

    private:
        MethodT m_method;
    };
}

#include "ConstructorWithMethod.inl"
