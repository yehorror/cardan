#pragma once

#include "ClassDetails.hpp"
#include "Converters/ToV8.hpp"

namespace cardan::classDetails
{
    template <class ClassT, typename ReturnType, typename... Args>
    Method<ClassT, ReturnType, Args...>::Method(const std::string& name, ReturnType (ClassT::*methodReference)(Args...))
        : m_methodReference(methodReference)
        , m_name(name)
    {
    }

    template <class ClassT, typename ReturnType, typename... Args>
    void Method<ClassT, ReturnType, Args...>::registerMember(
        Context& context,
        v8::Local<v8::ObjectTemplate>& objectTemplate
    )
    {
        auto funcName = convert(context, m_name, ToV8::ADLTag{}).template As<v8::String>();
        auto memberFuncTemplate = v8::FunctionTemplate::New(context.getIsolate());

        memberFuncTemplate->SetCallHandler([] (const v8::FunctionCallbackInfo<v8::Value>& callInfo)
        {
            auto self = callInfo.This();
            ClassT* classPtr = static_cast<ClassT*>(callInfo.This()->GetInternalField(0).As<v8::External>()->Value());
            // ClassT* classPtr = static_cast<ClassT*>(callInfo.This()->GetAlignedPointerFromInternalField(0));

            auto methodRef = *static_cast<MethodReferenceType*>(callInfo.Data().As<v8::External>()->Value());

            (classPtr->*methodRef)();

            //callInfo.GetReturnValue().Set(228);
        }, v8::External::New(context.getIsolate(), &m_methodReference));

        objectTemplate->Set(funcName, memberFuncTemplate);
    }
}
