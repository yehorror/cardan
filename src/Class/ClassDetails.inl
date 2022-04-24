#pragma once

#include "ClassDetails.hpp"

namespace cardan::classDetails
{
    template <class ClassT, typename ReturnType, typename... Args>
    Method<ClassT, ReturnType, Args...>::Method(const std::string& name, std::function<ReturnType(ClassT*, Args...)> function)
        : m_function(function)
        , m_name(name)
    {
    }

    template <class ClassT, typename ReturnType, typename... Args>
    void Method<ClassT, ReturnType, Args...>::registerMember()
    {
    }
}
