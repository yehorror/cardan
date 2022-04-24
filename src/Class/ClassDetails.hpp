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
}

#include "ClassDetails.inl"
