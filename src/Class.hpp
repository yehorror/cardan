#pragma once

#include <memory>
#include <string>
#include <functional>
#include <unordered_set>

#include "Class/ClassDetails.hpp"
#include "Converters/ToV8.hpp"

namespace cardan
{
    template <class ClassT>
    v8::Local<v8::Value> convert(Context& context, Class<ClassT>& classDef, ToV8::ADLTag);

    template <class ClassT>
    class Class
    {
    public:

        template <typename ReturnType, typename... Args>
        void method(const std::string& name, ReturnType(ClassT::*methodRef)(Args...));

    private:
        template <class T>
        friend v8::Local<v8::Value> cardan::convert(Context& context, Class<T>& classDef, ToV8::ADLTag);

        std::unordered_set<std::unique_ptr<classDetails::Member>> m_members;
    };
}

#include "Class.inl"
