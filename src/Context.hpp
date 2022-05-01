#pragma once

#include <string>
#include <unordered_set>

#include <functional>
#include <v8.h>
#include <libplatform/libplatform.h>

#include "Helper.hpp"
#include "Exceptions.hpp"

namespace cardan
{
    namespace ToV8
    {
        // This converter is forward-declared because it should have access to some cardan::Context`s internals
        struct ADLTag;

        template <class FuncReturnType, class... FuncArgs>
        v8::Local<v8::Value> convert(Context& context, std::function<FuncReturnType(FuncArgs...)> func, ToV8::ADLTag);
    }

    namespace classDetails
    {
        template <class ClassT>
        class DefaultConstructor;

        template <class ClassT, typename... Args>
        class ConstructorWithArgs;

        template <class ClassT, typename... Args>
        class ConstructorWithBindings;
    }

    template <class ClassT>
    class Class;

    class Value;
    class Object;
    class Array;
    class Function;

    // Is used to store some external values internally (e.g. functions, class pointers)
    struct ValueHolderBase
    {
        virtual ~ValueHolderBase() = default;
    };

    // Context of script execution
    // TODO:
    // * C++ classes binding into JS
    // * Consider moving of script compilation out of this class
    class Context
    {
    public:
        using ScriptRunResult = Value;

    public:
        Context();

        ScriptRunResult runScript(const std::string& code);

        Value get(const std::string& valueName);

        template <class ValueType>
        Value makeValue(ValueType&& value);

        Object global();

        v8::Isolate* getIsolate();
        v8::Local<v8::Context> getContext();

    public:

        template <class ValueType>
        void set(const std::string& name, ValueType&& value);

    private:

        template <class FuncReturnType, class... FuncArgs>
        friend v8::Local<v8::Value> cardan::ToV8::convert(Context& context, std::function<FuncReturnType(FuncArgs...)> func, ToV8::ADLTag);

        template <class ClassT>
        friend class classDetails::DefaultConstructor;

        template <class ClassT, typename... Args>
        friend class classDetails::ConstructorWithArgs;

        template <class ClassT, typename... Args>
        friend class classDetails::ConstructorWithBindings;

        void saveData(std::unique_ptr<ValueHolderBase> dataPtr);
        void removeData(ValueHolderBase* dataPtr);

        ScriptRunResult processRunResult(v8::MaybeLocal<v8::Value>& value, v8::TryCatch& tryCatchHandler);

    private:
        std::unique_ptr<v8::ArrayBuffer::Allocator> m_arrayBufferAllocator;

        // TODO Move isolate's guard declaration out of here
        // TODO Actually, move v8::Isolate outside of Context
        struct IsolateDisposer
        {
            void operator() (v8::Isolate* isolate);
        };

        std::unique_ptr<v8::Isolate, IsolateDisposer> m_isolate;

        v8::Isolate::Scope m_isolateScope;
        v8::HandleScope    m_handleScope;

        v8::Local<v8::Context> m_context;
        v8::Context::Scope m_contextScope;

        std::unordered_set<std::unique_ptr<ValueHolderBase>> m_values;
    };
}

#include "Context.inl"
