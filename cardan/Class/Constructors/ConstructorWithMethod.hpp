#pragma once

#include "Constructor.hpp"

namespace cardan::classDetails
{
    template <class ClassT, class MethodT>
    class ConstructorWithMethod : public Constructor<ClassT>
    {
    public:
        ConstructorWithMethod(MethodT&& method);
        ClassT* construct(Context& context, const v8::FunctionCallbackInfo<v8::Value>& callInfo) override;

    private:
        template <typename... Args>
        ClassT* createInstance(
            Context& context,
            const v8::FunctionCallbackInfo<v8::Value>& callInfo,
            std::function<ClassT*(Args...)> creationFunction
        );

    private:
        MethodT m_method;
    };
}

#include "ConstructorWithMethod.inl"
