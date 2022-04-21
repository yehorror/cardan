#pragma once

#include <vector>
#include <string>

#include "Value.hpp"
#include "Converters/ToV8.hpp"
#include "Helper.hpp"

namespace cardan
{
    class Value;

    // TODO This whole class consists on 90% of template functions
    // Consider removal of cpp file (it has only one simple method + constructr) and move it here or .inl file
    class Function
    {
    public:

        Function(v8::Local<v8::Function> function, Context& context);

        // TODO Move to inl file
        template<class...Args>
        Value call(Args... args);

    private:
        friend class Value;

        Value makeValue(v8::Local<v8::Value> value);

    public:
        v8::Local<v8::Function> m_function;
        Context& m_context;
    };
}

#include "Context.hpp"

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
        return makeValue(result);
    }
}
