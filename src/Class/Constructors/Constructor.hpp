#pragma once

#include "Context.hpp"

namespace cardan::classDetails
{
    template <class ClassT>
    struct ClassInstanceHolder : public ValueHolderBase
    {
        ClassInstanceHolder(std::unique_ptr<ClassT> classInstance, Context& context)
            : m_classInstance(std::move(classInstance))
            , m_context(context)
        {
        }

        std::unique_ptr<ClassT> m_classInstance;
        v8::Persistent<v8::External> m_persistentHolder;
        Context& m_context;
    };

    class Constructor
    {
    public:
        virtual void addConstructor(Context& context, v8::Local<v8::FunctionTemplate>& constructorFuncTemplate) = 0;
        virtual ~Constructor() = default;
    };
}
