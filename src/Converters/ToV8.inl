#pragma once
#include "Helper.hpp"
#include "Context.hpp"

namespace cardan::ToV8
{
    // TODO Research if we can implement same functions for lambdas, etc
    // Can we have some internal storage for these functions objects? And delete them when objects are deleted in JS context
    // TODO Also this method is too big. It is better to refactor it somehow
    /*
    template <class FuncReturnType, class... FuncArgs>
    v8::Local<v8::Value> convert(Context& context, std::function<FuncReturnType(FuncArgs...)>& func)
    {
        auto callbackLambda = [] (const v8::FunctionCallbackInfo<v8::Value>& info)
        {
            auto funcPtr = info.Data().As<v8::External>()->Value();
            auto& function = *static_cast<std::function<FuncReturnType(FuncArgs...)>*>(funcPtr);
            auto isolate = info.GetIsolate();

            if (std::tuple_size<std::tuple<FuncArgs...>>::value != info.Length())
            {
                isolate->ThrowException(ToV8::convert(isolate->GetCurrentContext(), "Invalid number of arguments"));
                return;
            }

            // TODO Catch possible exceptions here
            auto tupleWithArguments = details::packArguments<FuncArgs...>(info);

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

                    returnValue.Set(ToV8::convert(info.GetIsolate()->GetCurrentContext(), funcExecutionResult));
                }
            }
            catch (const std::exception& exception)
            {
                isolate->ThrowException(ToV8::convert(isolate->GetCurrentContext(), exception.what()));
            }
        };

        auto funcTemplate = v8::FunctionTemplate::New(
            context->GetIsolate(), callbackLambda, v8::External::New(context->GetIsolate(), &func)
        );

        return funcTemplate->GetFunction(context).ToLocalChecked();
    }
    */
}
