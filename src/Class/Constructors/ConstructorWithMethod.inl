#pragma once

#include "ConstructorWithMethod.hpp"
#include "Helper.hpp"

namespace cardan::classDetails
{
    template <class MethodT>
    struct ContextWithMethod : public ValueHolderBase
    {
        ContextWithMethod(Context& context, MethodT method)
            : m_context(context)
            , m_method(method)
        {
        }

        using MethodAsStdFunction = typename details::FunctionTraits<MethodT>::StdFunctionType;

        Context& m_context;
        MethodAsStdFunction m_method;
    };

    template <class ClassT, class MethodT>
    ConstructorWithMethod<ClassT, MethodT>::ConstructorWithMethod(MethodT&& method)
        : m_method(std::forward<MethodT>(method))
    {
    }

    template <class ClassT, class MethodT>
    void ConstructorWithMethod<ClassT, MethodT>::addConstructor(
        Context& context,
        v8::Local<v8::FunctionTemplate>& constructorFuncTemplate
    )
    {
        // TODO Need to avoid std::function, it has some kind of overhead
        using StdFunctionType = typename details::FunctionTraits<MethodT>::StdFunctionType;

        StdFunctionType methodAsStdFunction = m_method;

        addConstructorInternal(context, constructorFuncTemplate, methodAsStdFunction);
    }

    template <class ClassT, class MethodT>
    template <typename... Args>
    void ConstructorWithMethod<ClassT, MethodT>::addConstructorInternal(
            Context& context,
            v8::Local<v8::FunctionTemplate>& constructorFuncTemplate,
            std::function<ClassT*(Args...)> constructFunction
    )
    {
        auto contextWithMethod = std::make_unique<ContextWithMethod<MethodT>>(context, std::forward<MethodT>(m_method));
        auto contextWithMethodPtr = contextWithMethod.get();

        context.saveData(std::move(contextWithMethod));

        constructorFuncTemplate->SetCallHandler(
            v8Constructor<Args...>,
            v8::External::New(context.getIsolate(), contextWithMethodPtr)
        );
    }

    template <class ClassT, class MethodT>
    template <typename... Args>
    void ConstructorWithMethod<ClassT, MethodT>::v8Constructor(const v8::FunctionCallbackInfo<v8::Value>& callInfo)
    {
        v8::Isolate* isolate = callInfo.GetIsolate();

        if (!callInfo.IsConstructCall())
        {
            isolate->ThrowError(
                v8::String::NewFromUtf8(isolate, "Contructor should be called with 'new' keyword").ToLocalChecked()
            );
            return;
        }

        ContextWithMethod<MethodT>& contextWithMethod =
            *static_cast<ContextWithMethod<MethodT>*>(callInfo.Data().As<v8::External>()->Value());

        auto arguments = details::convertArgumentsFromV8<Args...>(contextWithMethod.m_context, callInfo);

        ClassT* instanceRawPtr = std::apply(contextWithMethod.m_method, arguments);

        auto instance = std::unique_ptr<ClassT>(instanceRawPtr);

        auto externalInstanceHolder = v8::External::New(isolate, instanceRawPtr);
        auto instanceHolder = std::make_unique<ClassInstanceHolder<ClassT>>(std::move(instance), contextWithMethod.m_context);

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

        contextWithMethod.m_context.saveData(std::move(instanceHolder));

        callInfo.This()->SetInternalField(0, externalInstanceHolder);
        callInfo.GetReturnValue().Set(callInfo.This());
    }
}
