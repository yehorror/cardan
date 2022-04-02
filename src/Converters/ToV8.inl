#include "Helper.hpp"

namespace cardan::converters
{
    // TODO Research if we can implement same functions for lambdas, etc
    // Can we have some internal storage for these functions objects? And delete them when objects are deleted in JS context
    // TODO Also this method is too big. It is better to refactor it somehow
    template <class FuncReturnType, class... FuncArgs>
    v8::Local<v8::Value> convert(v8::Local<v8::Context> context, std::function<FuncReturnType(FuncArgs...)>& func)
    {
        auto callbackLambda = [] (const v8::FunctionCallbackInfo<v8::Value>& info)
        {
            auto funcPtr = info.Data().As<v8::External>()->Value();
            auto& function = *static_cast<std::function<FuncReturnType(FuncArgs...)>*>(funcPtr);

            auto tupleWithArguments = details::packArguments<FuncArgs...>(info);
            auto isolate = info.GetIsolate();

            if (std::tuple_size<decltype(tupleWithArguments)>::value != info.Length())
            {
                isolate->ThrowException(converters::convert(isolate->GetCurrentContext(), "Invalid number of arguments"));
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

                    returnValue.Set(converters::convert(info.GetIsolate()->GetCurrentContext(), funcExecutionResult));
                }
            }
            catch (const std::exception& exception)
            {
                isolate->ThrowException(converters::convert(isolate->GetCurrentContext(), exception.what()));
            }
        };

        auto funcTemplate = v8::FunctionTemplate::New(
            context->GetIsolate(), callbackLambda, v8::External::New(context->GetIsolate(), &func)
        );

        return funcTemplate->GetFunction(context).ToLocalChecked();
    }
}
