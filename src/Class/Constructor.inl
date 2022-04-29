#pragma once

#include "Constructor.hpp"
#include "Context.hpp"

namespace cardan::classDetails
{
    template <class ClassT>
    struct ClassInstanceHolder : public ValueHolderBase
    {
        ClassInstanceHolder(std::unique_ptr<ClassT> classInstance, Context& context)
            : m_classInstance(std::move(classInstance))
            , m_context(context)
        {
        }

        std::unique_ptr<ClassT> m_classInstance;
        v8::Persistent<v8::External> m_persistentHolder;
        Context& m_context;
    };

    template <class ClassT>
    void DefaultConstructor<ClassT>::addConstructor(Context& context, v8::Local<v8::FunctionTemplate>& constructorFuncTemplate)
    {
        if constexpr (std::is_default_constructible_v<ClassT>)
        {
            constructorFuncTemplate->SetCallHandler([] (const v8::FunctionCallbackInfo<v8::Value>& callInfo)
            {
                v8::Isolate* isolate = callInfo.GetIsolate();
                Context& context = *static_cast<Context*>(callInfo.Data().As<v8::External>()->Value());

                if (!callInfo.IsConstructCall())
                {
                    isolate->ThrowError(
                        v8::String::NewFromUtf8(isolate, "Contructor should be called with 'new' keyword").ToLocalChecked()
                    );
                    return;
                }

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

            }, v8::External::New(context.getIsolate(), &context));
        }
    }

    template <class ClassT, typename... ConstructorArgs>
    void ConstructorWithArgs<ClassT, ConstructorArgs...>::addConstructor(
        Context& context,
        v8::Local<v8::FunctionTemplate>& constructorFuncTemplate
    )
    {
        constructorFuncTemplate->SetCallHandler([] (const v8::FunctionCallbackInfo<v8::Value>& callInfo)
        {
            v8::Isolate* isolate = callInfo.GetIsolate();
            Context& context = *static_cast<Context*>(callInfo.Data().As<v8::External>()->Value());

            if (!callInfo.IsConstructCall())
            {
                isolate->ThrowError(
                    v8::String::NewFromUtf8(isolate, "Contructor should be called with 'new' keyword").ToLocalChecked()
                );
                return;
            }

            auto arguments = details::convertArgumentsFromV8<ConstructorArgs...>(context, callInfo);

            auto instance = std::apply(std::make_unique<ClassT, ConstructorArgs...>, std::forward<decltype(arguments)>(arguments));

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

        }, v8::External::New(context.getIsolate(), &context));
    }
}
