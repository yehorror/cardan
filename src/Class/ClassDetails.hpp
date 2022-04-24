#pragma once

#include <string>
#include <functional>

namespace cardan::classDetails
{
    // Internal representation of some class member
    class Member
    {
    public:
        virtual void registerMember() = 0;
        virtual ~Member() = default;
    };

    template <class ClassT, typename ReturnType, typename... Args>
    class Method : public Member
    {
    public:
        Method(const std::string& name, std::function<ReturnType(ClassT*, Args...)> function);
        void registerMember() override;

    private:
        std::function<ReturnType(ClassT*, Args...)> m_function;
        const std::string m_name;
    };
}

#include "ClassDetails.inl"
