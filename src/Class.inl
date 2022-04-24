#pragma once

#include <iostream>

#include "Class.hpp"
#include "Converters/ToV8.hpp"

namespace cardan
{
    template <class ClassT>
    template <typename ReturnType, typename... Args>
    void Class<ClassT>::method(const std::string& name, ReturnType(ClassT::*methodRef)(Args...))
    {
        m_members.emplace(std::make_unique<classDetails::Method<ClassT, ReturnType, Args...>>(name, methodRef));
    }

    template <class ClassT>
    v8::Local<v8::Value> convert(Context& context, Class<ClassT>& classDef, ToV8::ADLTag)
    {
        auto constructorFuncTemplate = v8::FunctionTemplate::New(context.getIsolate());

        constructorFuncTemplate->SetCallHandler([] (const v8::FunctionCallbackInfo<v8::Value>& callInfo)
        {
            auto isolate = callInfo.GetIsolate();
            if (!callInfo.IsConstructCall())
            {
                isolate->ThrowError(
                    v8::String::NewFromUtf8(isolate, "Contructor should be called with 'new' keyword").ToLocalChecked()
                );
                return;
            }

            // FIXME Memory leak here
            callInfo.This()->SetInternalField(0, v8::External::New(isolate, new ClassT));
            callInfo.GetReturnValue().Set(callInfo.This());
        });

        auto instanceTemplate = constructorFuncTemplate->InstanceTemplate();

        for (const auto& member : classDef.m_members)
        {
            member->registerMember(context, instanceTemplate);
        }

        constructorFuncTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        return constructorFuncTemplate->GetFunction(context.getContext()).ToLocalChecked();
    }
}
