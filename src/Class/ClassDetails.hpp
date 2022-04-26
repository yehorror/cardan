#pragma once

#include <string>
#include <functional>
#include <v8.h>

namespace cardan
{
    class Context;
}

namespace cardan::classDetails
{
    // Internal representation of some class member
    class Member
    {
    public:
        virtual void registerMember(Context& context, v8::Local<v8::ObjectTemplate>& objectTemplate) = 0;
        virtual ~Member() = default;
    };

    template <class ClassT, typename ReturnType, typename... Args>
    class Method : public Member
    {
    public:
        Method(const std::string& name, ReturnType(ClassT::* methodReference )(Args...));
        void registerMember(Context& context, v8::Local<v8::ObjectTemplate>& objectTemplate) override;

    private:
        using MethodReferenceType = ReturnType(ClassT::*)(Args...);

        MethodReferenceType m_methodReference;
        const std::string m_name;
    };

    template <class ClassT, typename PropertyType>
    class PropertyGetterSetter : public Member
    {
    public:
        PropertyGetterSetter(
            const std::string& name,
            PropertyType(ClassT::*getter)(),
            void(ClassT::*setter)(PropertyType)
        );

        void registerMember(Context& context, v8::Local<v8::ObjectTemplate>& objectTemplate) override;

    private:
        using MethodGetterType = PropertyType(ClassT::*)();
        using MethodSetterType = void(ClassT::*)(PropertyType);

        MethodGetterType m_getter;
        MethodSetterType m_setter;

        const std::string m_name;
    };

    template <class ClassT, typename PropertyType>
    class Property : public Member
    {
    public:
        Property(const std::string& name, PropertyType ClassT::* memberReference);

        void registerMember(Context& context, v8::Local<v8::ObjectTemplate>& objectTemplate) override;

    private:
        using PropertyMemberType = PropertyType ClassT::*;

        PropertyMemberType m_memberReference;
        const std::string m_name;
    };
}

#include "ClassDetails.inl"
