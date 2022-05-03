#pragma once

#include "ConstructorWithArgs.hpp"

namespace cardan::classDetails
{
    template <class ClassT, typename... ConstructorArgs>
    void ConstructorWithArgs<ClassT, ConstructorArgs...>::addConstructor(
        Context& context,
        v8::Local<v8::FunctionTemplate>& constructorFuncTemplate
    )
    {
        constructorFuncTemplate->SetCallHandler(v8Constructor, v8::External::New(context.getIsolate(), &context));
    }

    template <class ClassT, typename... ConstructorArgs>
    void ConstructorWithArgs<ClassT, ConstructorArgs...>::v8Constructor(
        const v8::FunctionCallbackInfo<v8::Value>& callInfo
    )
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

       auto arguments = details::convertArgumentsFromV8<ConstructorArgs...>(context, callInfo);

       auto instance = std::apply(
           std::make_unique<ClassT, ConstructorArgs...>,
           std::forward<decltype(arguments)>(arguments)
       );

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