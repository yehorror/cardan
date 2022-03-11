#pragma once

#include <v8.h>
#include <string>
#include <functional>

#include "Helper.hpp"

namespace cardan::converters
{
    v8::Local<v8::Value> convert(v8::Isolate* isolate, v8::Local<v8::Context> context, int value);
    v8::Local<v8::Value> convert(v8::Isolate* isolate, v8::Local<v8::Context> context, double value);
    v8::Local<v8::Value> convert(v8::Isolate* isolate, v8::Local<v8::Context> context, const std::string& value);
    v8::Local<v8::Value> convert(v8::Isolate* isolate, v8::Local<v8::Context> /*context*/, Function value);

    // TODO Move out of here (maybe FunctionConverter.h/.inl)
    template <class FuncReturnType, class... FuncArgs>
    v8::Local<v8::Value> convert(v8::Isolate* isolate, v8::Local<v8::Context> context, std::function<FuncReturnType(FuncArgs...)>& func)
    {
        auto callbackLambda = [] (const v8::FunctionCallbackInfo<v8::Value>& info)
        {
            auto funcPtr = info.Data().As<v8::External>()->Value();
            auto& function = *static_cast<std::function<FuncReturnType(FuncArgs...)>*>(funcPtr);

            auto tupleWithArguments = details::packArguments<FuncArgs...>(info);
            auto isolate = info.GetIsolate();

            if (std::tuple_size<decltype(tupleWithArguments)>::value != info.Length())
            {
                isolate->ThrowException(converters::convert(isolate, isolate->GetCurrentContext(), "Invalid number of arguments"));
                return;
            }

            try
            {
                if constexpr (std::is_same_v<void, FuncReturnType>)
                {
                    // If bound function has 'void' return type, just call function and don't bother about result
                    std::apply(function, tupleWithArguments);
                }
                else
                {
                    // Otherwise, convert this value to V8's return value
                    auto funcExecutionResult = std::apply(function, tupleWithArguments);
                    auto returnValue = info.GetReturnValue();

                    details::convertValueToV8ReturnValue(info.GetIsolate(), funcExecutionResult, returnValue);
                }
            }
            catch (const std::exception& exception)
            {
                isolate->ThrowException(converters::convert(isolate, isolate->GetCurrentContext(), exception.what()));
            }
        };

        auto funcTemplate = v8::FunctionTemplate::New(
            isolate, callbackLambda, v8::External::New(isolate, &func)
        );

        return funcTemplate->GetFunction(context).ToLocalChecked();
    }
}
