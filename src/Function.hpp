#pragma once

#include <vector>

#include "Helper.hpp"
#include "Value.hpp"

namespace cardan
{
    static v8::Local<v8::Value> convertArgumentToV8Value(v8::Isolate* isolate, int argument)
    {
        return v8::Integer::New(isolate, argument);
    }

    template <size_t Idx=0, class... TupleT>
    static void argumentsToVector(const std::tuple<TupleT...>& arguments, std::vector<v8::Local<v8::Value>>& values, v8::Isolate* isolate)
    {
        values.push_back(
            convertArgumentToV8Value(isolate, std::get<Idx>(arguments))
        );

        if constexpr (Idx < (std::tuple_size<std::tuple<TupleT...>>::value - 1))
        {
            argumentsToVector<Idx + 1, TupleT...>(arguments, values, isolate);
        }
    }

    template <>
    void argumentsToVector(const std::tuple<>&, std::vector<v8::Local<v8::Value>>&, v8::Isolate*)
    {
    }

    class Function;
    class Value;

    class Function
    {
    public:

        template<class...Args>
        Value call(Args... args)
        {
            std::vector<v8::Local<v8::Value>> argumentsVector;

            auto argsTuple = std::make_tuple(args...);

            argumentsToVector(argsTuple, argumentsVector, m_isolate);

            auto result = m_function->Call(m_context, m_context->Global(), argumentsVector.size(), argumentsVector.data()).ToLocalChecked();
            return makeValue(result);
        }

    private:
        friend class Value;
        Function(v8::Local<v8::Function> function, v8::Isolate* isolate, v8::Local<v8::Context>& context);
        Value makeValue(v8::Local<v8::Value> value);

    public:
        v8::Local<v8::Function> m_function;
        v8::Isolate* m_isolate;
        v8::Local<v8::Context>& m_context;
    };
}
