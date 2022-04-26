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

        struct ContextWithMethodReference
        {
            MethodReferenceType m_methodReference;
            Context& m_context;
        };

        memberFuncTemplate->SetCallHandler([] (const v8::FunctionCallbackInfo<v8::Value>& callInfo)
        {
            v8::Local<v8::Object> self = callInfo.This();
            auto& contextWithMethodReference = *static_cast<ContextWithMethodReference*>(callInfo.Data().As<v8::External>()->Value());

            ClassT* classPtr = static_cast<ClassT*>(self->GetInternalField(0).As<v8::External>()->Value());
            // ClassT* classPtr = static_cast<ClassT*>(callInfo.This()->GetAlignedPointerFromInternalField(0));

            auto arguments = details::convertArgumentsFromV8<Args...>(contextWithMethodReference.m_context, callInfo);

            if constexpr (std::is_same_v<void, ReturnType>)
            {
                // Cannot call non-static member functions directly in std::apply, so had to do this 'proxy'
                auto memberFunctionCallProxy = [&] (Args... arguments)
                {
                    ((*classPtr).*(contextWithMethodReference.m_methodReference))(arguments...);
                };

                std::apply(memberFunctionCallProxy, arguments);
            }
            else
            {
                auto memberFunctionCallProxy = [&] (Args... arguments)
                {
                    return ((*classPtr).*(contextWithMethodReference.m_methodReference))(arguments...);
                };

                auto result = std::apply(memberFunctionCallProxy, arguments);
                callInfo.GetReturnValue().Set(result);
            }

        }, v8::External::New(context.getIsolate(), new ContextWithMethodReference {m_methodReference, context}));

        objectTemplate->Set(funcName, memberFuncTemplate);
    }

    template <class ClassT, typename PropertyType>
    PropertyGetterSetter<ClassT, PropertyType>::PropertyGetterSetter(
        const std::string& name,
        PropertyType(ClassT::*getter)(),
        void(ClassT::*setter)(PropertyType)
    )
        : m_name(name)
        , m_getter(getter)
        , m_setter(setter)
    {
    }

    template <class ClassT, typename PropertyType>
    void PropertyGetterSetter<ClassT, PropertyType>::registerMember(
        Context& context,
        v8::Local<v8::ObjectTemplate>& objectTemplate
    )
    {
        struct ContextWithMethodsReferences
        {
            MethodGetterType m_getter;
            MethodSetterType m_setter;
            Context& m_context;
        };

        auto getterLambda = [] (v8::Local<v8::String> /*name*/, const v8::PropertyCallbackInfo<v8::Value>& propertyInfo)
        {
            v8::Local<v8::Object> self = propertyInfo.This();
            ClassT* classPtr = static_cast<ClassT*>(self->GetInternalField(0).As<v8::External>()->Value());

            ContextWithMethodsReferences& contextWithMethodsReferences =
                *static_cast<ContextWithMethodsReferences*>(propertyInfo.Data().As<v8::External>()->Value());

            auto value = (classPtr->*(contextWithMethodsReferences.m_getter))();

            propertyInfo.GetReturnValue().Set(convert(contextWithMethodsReferences.m_context, value, ToV8::ADLTag{}));
        };

        auto setterLambda = [] (
            v8::Local<v8::String> /*name*/,
            v8::Local<v8::Value> newValue,
            const v8::PropertyCallbackInfo<void>& propertyInfo
        )
        {
            v8::Local<v8::Object> self = propertyInfo.This();
            ClassT* classPtr = static_cast<ClassT*>(self->GetInternalField(0).As<v8::External>()->Value());

            ContextWithMethodsReferences& contextWithMethodsReferences =
                *static_cast<ContextWithMethodsReferences*>(propertyInfo.Data().As<v8::External>()->Value());

            auto convertedValue = convert(contextWithMethodsReferences.m_context, newValue, FromV8::To<PropertyType>{});

            (classPtr->*(contextWithMethodsReferences.m_setter))(convertedValue);
        };

        auto v8Name = convert(context, m_name, ToV8::ADLTag{}).As<v8::String>();

        objectTemplate->SetAccessor(
            v8Name,
            getterLambda,
            setterLambda,
            v8::External::New(context.getIsolate(), new ContextWithMethodsReferences{ m_getter, m_setter, context })
        );
    }

    template <class ClassT, typename PropertyType>
    Property<ClassT, PropertyType>::Property(const std::string& name, PropertyType ClassT::* memberReference)
        : m_memberReference(memberReference)
        , m_name(name)
    {
    }

    template <class ClassT, typename PropertyType>
    void Property<ClassT, PropertyType>::registerMember(Context& context, v8::Local<v8::ObjectTemplate>& objectTemplate)
    {
        struct ContextWithMethodsReferences
        {
            PropertyMemberType m_memberReference;
            Context& m_context;
        };

        auto getterLambda = [] (v8::Local<v8::String> /*name*/, const v8::PropertyCallbackInfo<v8::Value>& propertyInfo)
        {
            v8::Local<v8::Object> self = propertyInfo.This();
            ClassT* classPtr = static_cast<ClassT*>(self->GetInternalField(0).As<v8::External>()->Value());

            ContextWithMethodsReferences& contextWithMethodsReferences =
                *static_cast<ContextWithMethodsReferences*>(propertyInfo.Data().As<v8::External>()->Value());

            auto value = (classPtr->*(contextWithMethodsReferences.m_memberReference));

            propertyInfo.GetReturnValue().Set(convert(contextWithMethodsReferences.m_context, value, ToV8::ADLTag{}));
        };

        auto setterLambda = [] (
            v8::Local<v8::String> /*name*/,
            v8::Local<v8::Value> newValue,
            const v8::PropertyCallbackInfo<void>& propertyInfo
        )
        {
            v8::Local<v8::Object> self = propertyInfo.This();
            ClassT* classPtr = static_cast<ClassT*>(self->GetInternalField(0).As<v8::External>()->Value());

            ContextWithMethodsReferences& contextWithMethodsReferences =
                *static_cast<ContextWithMethodsReferences*>(propertyInfo.Data().As<v8::External>()->Value());

            auto convertedValue = convert(contextWithMethodsReferences.m_context, newValue, FromV8::To<PropertyType>{});

            (classPtr->*(contextWithMethodsReferences.m_memberReference)) = convertedValue;
        };

        auto v8Name = convert(context, m_name, ToV8::ADLTag{}).As<v8::String>();

        objectTemplate->SetAccessor(
            v8Name,
            getterLambda,
            setterLambda,
            v8::External::New(context.getIsolate(), new ContextWithMethodsReferences{ m_memberReference, context })
        );
    }
}
