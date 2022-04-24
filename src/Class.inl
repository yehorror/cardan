#pragma once

#include <cassert>

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
        auto funcTemplate = v8::FunctionTemplate::New(context.getIsolate());

        auto funcName = convert(context, "parameterlessMethod", ToV8::ADLTag{}).template As<v8::String>();

        auto memberFuncTemplate = v8::FunctionTemplate::New(context.getIsolate());

        memberFuncTemplate->SetCallHandler([] (const v8::FunctionCallbackInfo<v8::Value>& callInfo)
        {
            auto self = callInfo.This();
            ClassT* classPtr = static_cast<ClassT*>(callInfo.This()->GetInternalField(0).As<v8::External>()->Value());
            // ClassT* classPtr = static_cast<ClassT*>(callInfo.This()->GetAlignedPointerFromInternalField(0));
            classPtr->parameterlessMethod();
            //callInfo.GetReturnValue().Set(228);
        });

        funcTemplate->PrototypeTemplate()->Set(funcName, memberFuncTemplate);
        funcTemplate->PrototypeTemplate()->SetInternalFieldCount(2);

        struct PrototypeData
        {
            v8::Local<v8::ObjectTemplate> m_prototype;
        };

        // TODO Fix memory leak here (new PrototypeData)
        funcTemplate->SetCallHandler([] (const v8::FunctionCallbackInfo<v8::Value>& callInfo)
        {
            auto isolate = callInfo.GetIsolate();
            auto prototypeData = static_cast<PrototypeData*>(callInfo.Data().As<v8::External>()->Value());

            auto newInstance = prototypeData->m_prototype->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();
            newInstance->SetInternalField(0, v8::External::New(isolate, new ClassT));

            callInfo.GetReturnValue().Set(newInstance);
        }, v8::External::New(context.getIsolate(), new PrototypeData{ funcTemplate->PrototypeTemplate() }));

        return funcTemplate->GetFunction(context.getContext()).ToLocalChecked();
    }
}
