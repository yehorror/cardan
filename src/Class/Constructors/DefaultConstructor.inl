#pragma once

#include "DefaultConstructor.hpp"

namespace cardan::classDetails
{
    template <class ClassT>
    void DefaultConstructor<ClassT>::addConstructor(
        Context& context,
        v8::Local<v8::FunctionTemplate>& constructorFuncTemplate
    )
    {
        if constexpr (std::is_default_constructible_v<ClassT>)
        {
            constructorFuncTemplate->SetCallHandler(v8Constructor, v8::External::New(context.getIsolate(), &context));
        }
        else
        {
            throw std::logic_error("No constructor provided for non-default constructible type");
        }
    }

    template <class ClassT>
    void DefaultConstructor<ClassT>::v8Constructor(const v8::FunctionCallbackInfo<v8::Value>& callInfo)
    {
        v8::Isolate* isolate = callInfo.GetIsolate();

        if (!callInfo.IsConstructCall())
        {
            isolate->ThrowError(
                v8::String::NewFromUtf8(isolate, "Contructor should be called with 'new' keyword").ToLocalChecked()
            );
            return;
        }

        Context& context = *static_cast<Context*>(callInfo.Data().As<v8::External>()->Value());

        auto instance = std::make_unique<ClassT>();
        ClassT* instanceRawPtr = instance.get();

        auto externalInstanceHolder = v8::External::New(isolate, instanceRawPtr);

        auto instanceHolder = std::make_unique<ClassInstanceHolder<ClassT>>(std::move(instance), context);

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

        context.saveData(std::move(instanceHolder));

        callInfo.This()->SetInternalField(0, externalInstanceHolder);
        callInfo.GetReturnValue().Set(callInfo.This());
    }
}
