#pragma once

#include <type_traits>

#include "cardan/Class.hpp"
#include "cardan/Converters/ToV8.hpp"
#include "cardan/Class/ConstructMethod.hpp"
#include "cardan/Class/Constructors/DefaultConstructor.hpp"
#include "cardan/Class/Constructors/ConstructorWithArgs.hpp"
#include "cardan/Class/Constructors/ConstructorWithMethod.hpp"

namespace cardan
{
    template <class ClassT>
    Class<ClassT>::Class()
        : m_constructor(std::make_unique<classDetails::DefaultConstructor<ClassT>>())
        , m_destructor([] (ClassT* instancePtr) { delete instancePtr; })
    {
    }

    template <class ClassT>
    template <typename... Args>
    void Class<ClassT>::constructor()
    {
        m_constructor.reset(new classDetails::ConstructorWithArgs<ClassT, Args...>());
    }

    template <class ClassT>
    template <class MethodT>
    void Class<ClassT>::constructorMethod(MethodT&& method)
    {
        m_constructor.reset(
            new classDetails::ConstructorWithMethod<ClassT, MethodT>(std::forward<MethodT>(method))
        );
    }

    template <class ClassT>
    template <class MethodT>
    void Class<ClassT>::destructionMethod(MethodT&& method)
    {
        m_destructor = method;
    }

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

        auto constructionContext = std::make_unique<classDetails::ConstructionContext<ClassT>>(
            std::move(classDef.m_constructor),
            classDef.m_destructor,
            context
        );

        auto constructionContextRaw = constructionContext.get();

        context.saveData(std::move(constructionContext));

        constructorFuncTemplate->SetCallHandler(
            classDetails::instanceConstructor<ClassT>,
            v8::External::New(context.getIsolate(), constructionContextRaw)
        );

        auto instanceTemplate = constructorFuncTemplate->InstanceTemplate();

        for (const auto& member : classDef.m_members)
        {
            member->registerMember(context, instanceTemplate);
        }

        constructorFuncTemplate->InstanceTemplate()->SetInternalFieldCount(1);

        return constructorFuncTemplate->GetFunction(context.getContext()).ToLocalChecked();
    }
}
