#pragma once

#include "Class.hpp"

namespace cardan
{
    template <class ClassT>
    template <typename ReturnType, typename... Args>
    void Class<ClassT>::method(const std::string& name, ReturnType(ClassT::*methodRef)(Args...))
    {

    }
}
