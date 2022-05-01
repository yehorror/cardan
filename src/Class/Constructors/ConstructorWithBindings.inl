#pragma once

#include "ConstructorWithBindings.hpp"

namespace cardan::classDetails
{
    template <class ClassT, typename... Args>
    ConstructorWithBindings<ClassT, Args...>::ConstructorWithBindings(Args&&... args)
        : m_args(std::forward<Args>(args)...)
    {
    }

    template <class ClassT, typename... Args>
    void ConstructorWithBindings<ClassT, Args...>::addConstructor(
        Context& context,
        v8::Local<v8::FunctionTemplate>& constructorFuncTemplate
    )
    {
        struct ContextWithArgs
        {
            ContextWithArgs(Context& context, std::tuple<Args...>&& args)
                : m_context(context)
                , m_args(std::forward<std::tuple<Args...>>(args))
            {
            }

            Context& m_context;
            std::tuple<Args...> m_args;
        };

        constructorFuncTemplate->SetCallHandler([] (const v8::FunctionCallbackInfo<v8::Value>& callInfo)
        {
            v8::Isolate* isolate = callInfo.GetIsolate();
            ContextWithArgs& contextWithArgs = *static_cast<ContextWithArgs*>(callInfo.Data().As<v8::External>()->Value());

            if (!callInfo.IsConstructCall())
            {
                isolate->ThrowError(
                    v8::String::NewFromUtf8(isolate, "Contructor should be called with 'new' keyword").ToLocalChecked()
                );
                return;
            }

            // auto arguments = details::convertArgumentsFromV8<Args...>(contextWihtArgs.context, callInfo);

            auto instance = std::apply(
                std::make_unique<ClassT, Args...>,
                std::forward<std::tuple<Args...>>(contextWithArgs.m_args)
            );

            ClassT* instanceRawPtr = instance.get();

            auto externalInstanceHolder = v8::External::New(isolate, instanceRawPtr);
            auto instanceHolder = std::make_unique<ClassInstanceHolder<ClassT>>(std::move(instance), contextWithArgs.m_context);

            instanceHolder->m_persistentHolder.Reset(isolate, externalInstanceHolder);
            instanceHolder->m_persistentHolder.SetWeak(
                instanceHolder.get(),
                [] (const v8::WeakCallbackInfo<ClassInstanceHolder<ClassT>>& info)
                {
                    ClassInstanceHolder<ClassT>* instanceHolder = info.GetParameter();
                    instanceHolder->m_classInstance.reset();
                    instanceHolder->m_persistentHolder.ClearWeak();
                    instanceHolder->m_context.removeData(instanceHolder);
                },
                v8::WeakCallbackType::kFinalizer
            );

            contextWithArgs.m_context.saveData(std::move(instanceHolder));

            callInfo.This()->SetInternalField(0, externalInstanceHolder);
            callInfo.GetReturnValue().Set(callInfo.This());

        }, v8::External::New(context.getIsolate(), new ContextWithArgs(context, std::forward<std::tuple<Args...>>(m_args))));
    }
}
