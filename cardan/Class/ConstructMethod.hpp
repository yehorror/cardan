#pragma once

#include "Constructors/Constructor.hpp"

namespace cardan::classDetails
{
    template <class ClassT>
    struct ClassInstanceHolder : public ValueHolderBase
    {
        ClassInstanceHolder(ClassT* instance, std::function<void(ClassT*)> deleter, Context& context)
            : m_instance(instance, std::move(deleter))
            , m_context(context)
        {
        }

        std::unique_ptr<ClassT, std::function<void(ClassT*)>> m_instance;
        Context& m_context;

        v8::Persistent<v8::External> m_persistentHolder;
    };

    template <class ClassT>
    struct ConstructionContext : public ValueHolderBase
    {
        ConstructionContext(
            std::unique_ptr<Constructor<ClassT>> constructor,
            std::function<void(ClassT*)> deleter,
            Context& context
        )
            : m_constructor(std::move(constructor))
            , m_deleter(std::move(deleter))
            , m_context(context)
        {
        }

        std::unique_ptr<Constructor<ClassT>> m_constructor;
        std::function<void(ClassT*)> m_deleter;
        Context& m_context;
    };

    template <class ClassT>
    void instanceConstructor(const v8::FunctionCallbackInfo<v8::Value>& callInfo)
    {
        v8::Isolate* isolate = callInfo.GetIsolate();

        if (!callInfo.IsConstructCall())
        {
            isolate->ThrowError(
                v8::String::NewFromUtf8(isolate, "Contructor should be called with 'new' keyword").ToLocalChecked()
            );
            return;
        }

        ConstructionContext<ClassT>& constructionContext =
            *static_cast<ConstructionContext<ClassT>*>(callInfo.Data().As<v8::External>()->Value());

        ClassT* instanceRawPtr = nullptr;
        try
        {
            instanceRawPtr = constructionContext.m_constructor->construct(constructionContext.m_context, callInfo);
        }
        catch (const std::exception& exception)
        {
            isolate->ThrowError(v8::String::NewFromUtf8(isolate, exception.what()).ToLocalChecked());
            return;
        }

        auto externalInstanceHolder = v8::External::New(isolate, instanceRawPtr);

        auto instanceHolder = std::make_unique<ClassInstanceHolder<ClassT>>(
            instanceRawPtr,
            constructionContext.m_deleter,
            constructionContext.m_context
        );

        instanceHolder->m_persistentHolder.Reset(isolate, externalInstanceHolder);

        instanceHolder->m_persistentHolder.SetWeak(
            instanceHolder.get(),
            [] (const v8::WeakCallbackInfo<ClassInstanceHolder<ClassT>>& info)
            {
                ClassInstanceHolder<ClassT>* instanceHolder = info.GetParameter();
                instanceHolder->m_persistentHolder.ClearWeak();
                instanceHolder->m_context.removeData(instanceHolder);
            },
            v8::WeakCallbackType::kFinalizer
        );

        constructionContext.m_context.saveData(std::move(instanceHolder));

        callInfo.This()->SetInternalField(0, externalInstanceHolder);
        callInfo.GetReturnValue().Set(callInfo.This());
    }
}
