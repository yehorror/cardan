#pragma once

#include "cardan/Context.hpp"

namespace cardan
{
    template<class...Args>
    Value Function::call(Args... args)
    {
        std::vector<v8::Local<v8::Value>> argumentsVector;

        auto argsTuple = std::make_tuple(args...);

        details::forEachElementInTuple(argsTuple, [&] (const auto& argument)
        {
            argumentsVector.push_back(convert(m_context, argument, ToV8::ADLTag{}));
        });

        auto result = m_function->Call(
            m_context.getContext(),
            m_context.getContext()->Global(),
            argumentsVector.size(),
            argumentsVector.data()
        ).ToLocalChecked();

        return Value(result, m_context);
    }
}
