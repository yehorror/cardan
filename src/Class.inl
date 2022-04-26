#pragma once

#include <iostream>

#include "Class.hpp"
#include "Converters/ToV8.hpp"

namespace cardan
{
    template <class ClassT>
    struct ClassInstanceHolder : public ValueHolderBase
    {
        ClassInstanceHolder(std::unique_ptr<ClassT> classInstance)
            : m_classInstance(std::move(classInstance))
        {
        }
        std::unique_ptr<ClassT> m_classInstance;
    };

    template <class ClassT>
    template <typename ReturnType, typename... Args>
    void Class<ClassT>::method(const std::string& name, ReturnType(ClassT::*methodRef)(Args...))
    {
        m_members.emplace(std::make_unique<classDetails::Method<ClassT, ReturnType, Args...>>(name, methodRef));
    }

    template <class ClassT>
    template <typename PropertyGetterSetterType>
    void Class<ClassT>::property(
        const std::string& name,
        PropertyGetterSetterType(ClassT::*getter)(),
        void(ClassT::*setter)(PropertyGetterSetterType)
    )
    {
        m_members.emplace(std::make_unique<classDetails::PropertyGetterSetter<ClassT, PropertyGetterSetterType>>(name, getter, setter));
    }

    template <class ClassT>
    template <typename PropertyType>
    void Class<ClassT>::property(const std::string& name, PropertyType ClassT::* field)
    {
        m_members.emplace(std::make_unique<classDetails::Property<ClassT, PropertyType>>(name, field));
    }

    template <class ClassT>
    v8::Local<v8::Value> convert(Context& context, Class<ClassT>& classDef, ToV8::ADLTag)
    {
        auto constructorFuncTemplate = v8::FunctionTemplate::New(context.getIsolate());

        // TODO Move to static function
        constructorFuncTemplate->SetCallHandler([] (const v8::FunctionCallbackInfo<v8::Value>& callInfo)
        {
            auto isolate = callInfo.GetIsolate();
            auto& context = *static_cast<Context*>(callInfo.Data().As<v8::External>()->Value());

            if (!callInfo.IsConstructCall())
            {
                isolate->ThrowError(
                    v8::String::NewFromUtf8(isolate, "Contructor should be called with 'new' keyword").ToLocalChecked()
                );
                return;
            }

            auto instanceHolder = std::make_unique<ClassInstanceHolder<ClassT>>(std::make_unique<ClassT>());
            auto instancePtr = instanceHolder->m_classInstance.get();

            context.saveData(std::move(instanceHolder));

            // FIXME Memory leak here
            callInfo.This()->SetInternalField(0, v8::External::New(isolate, instancePtr));
            callInfo.GetReturnValue().Set(callInfo.This());

        }, v8::External::New(context.getIsolate(), &context));

        auto instanceTemplate = constructorFuncTemplate->InstanceTemplate();

        for (const auto& member : classDef.m_members)
        {
            member->registerMember(context, instanceTemplate);
        }

        constructorFuncTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        return constructorFuncTemplate->GetFunction(context.getContext()).ToLocalChecked();
    }
}
