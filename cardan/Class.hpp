#pragma once

#include <memory>
#include <string>
#include <functional>
#include <unordered_set>

#include "cardan/Class/ClassDetails.hpp"
#include "cardan/Class/Constructors/Constructor.hpp"
#include "cardan/Converters/ToV8.hpp"

namespace cardan
{
    template <class ClassT>
    v8::Local<v8::Value> convert(Context& context, Class<ClassT>& classDef, ToV8::ADLTag);

    template <class ClassT>
    class Class
    {
    public:
        Class();

        template <typename... Args>
        void constructor();

        template <class MethodT>
        void constructorMethod(MethodT&& method);

        template <class MethodT>
        void destructionMethod(MethodT&& method);

        template <typename ReturnType, typename... Args>
        void method(const std::string& name, ReturnType(ClassT::*methodRef)(Args...));

        template <typename PropertyType>
        void property(const std::string& name, PropertyType(ClassT::*getter)(), void(ClassT::*setter)(PropertyType));

        template <typename PropertyType>
        void property(const std::string& name, PropertyType ClassT::* field);

    private:
        template <class T>
        friend v8::Local<v8::Value> cardan::convert(Context& context, Class<T>& classDef, ToV8::ADLTag);

    private:
        std::unique_ptr<classDetails::Constructor<ClassT>> m_constructor;
        std::unordered_set<std::unique_ptr<classDetails::Member>> m_members;

        std::function<void(ClassT*)> m_destructor;
    };
}

#include "Class.inl"
