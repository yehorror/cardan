#pragma once

#include <vector>
#include <string>

#include "Value.hpp"

namespace cardan
{
    namespace details
    {
        template <class Type>
        static Type convertArgumentFromV8Value(v8::Local<v8::Context> context, v8::Local<v8::Value> value);
    }

    static v8::Local<v8::Value> convertArgumentToV8Value(v8::Isolate* isolate, int argument)
    {
        return v8::Integer::New(isolate, argument);
    }

    static v8::Local<v8::Value> convertArgumentToV8Value(v8::Isolate* isolate, const std::string& argument)
    {
        return v8::String::NewFromUtf8(isolate, argument.c_str()).ToLocalChecked();
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

            argumentsToVector(argsTuple, argumentsVector, m_context->GetIsolate());

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

        friend Function details::convertArgumentFromV8Value<>(v8::Local<v8::Context> context, v8::Local<v8::Value> value);

        Function(v8::Local<v8::Function> function, v8::Local<v8::Context>& context);

        Value makeValue(v8::Local<v8::Value> value);

    public:
        v8::Local<v8::Function> m_function;
        v8::Local<v8::Context> m_context;
    };
}
