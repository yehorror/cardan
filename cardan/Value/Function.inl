#pragma once

#include "cardan/Context.hpp"

namespace cardan
{
    template<class...Args>
    Value Function::call(Args&&... args)
    {
        auto arguments = argumentsToV8(std::forward<Args>(args)...);

        auto result = m_function->Call(
            m_context.getContext(),
            m_context.getContext()->Global(),
            arguments.size(),
            arguments.data()
        ).ToLocalChecked();

        return Value(result, m_context);
    }

    template <class...Args>
    Value Function::callAsConstructor(Args&&... args)
    {
        auto arguments = argumentsToV8(std::forward<Args>(args)...);

        auto result = m_function->CallAsConstructor(
            m_context.getContext(),
            arguments.size(),
            arguments.data()
        ).ToLocalChecked();

        return Value(result, m_context);
    }

    template <class... Args>
    std::vector<v8::Local<v8::Value>> Function::argumentsToV8(Args&&... args)
    {
        std::vector<v8::Local<v8::Value>> argumentsVector;

        auto argsTuple = std::make_tuple(std::forward<Args>(args)...);

        details::forEachElementInTuple(argsTuple, [&] (auto&& argument)
        {
            argumentsVector.push_back(convert(m_context, std::forward<decltype(argument)>(argument), ToV8::ADLTag{}));
        });

        return argumentsVector;
    }
}
