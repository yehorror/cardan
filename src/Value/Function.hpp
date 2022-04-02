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

        // TODO Move to inl file
        template<class...Args>
        Value call(Args... args)
        {
            std::vector<v8::Local<v8::Value>> argumentsVector;

            auto argsTuple = std::make_tuple(args...);

            details::forEachElementInTuple(argsTuple, [&] (const auto& argument)
            {
                argumentsVector.push_back(converters::convert(m_context, argument));
            });

            auto result = m_function->Call(m_context, m_context->Global(), argumentsVector.size(), argumentsVector.data()).ToLocalChecked();
            return makeValue(result);
        }

    private:
        friend class Value;

        // "friends" are weird here - it forces us to do forward-declarations of convertors above
        // These constructions definitely makes code more messy
        // TODO Consider changing this semantics; possible approaches:
        //  - Make constructors public;
        //  - Somehow make possible creation of these things via Context class (shall it lead to less "friend"liness?)
        //  - Re-design everything in library?
        // Need to get rid of this

        friend Function converters::convertArgumentFromV8Value<>(v8::Local<v8::Context> context, v8::Local<v8::Value> value);

        Function(v8::Local<v8::Function> function, v8::Local<v8::Context>& context);

        Value makeValue(v8::Local<v8::Value> value);

    public:
        v8::Local<v8::Function> m_function;
        v8::Local<v8::Context> m_context;
    };
}
