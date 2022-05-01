#pragma once

#include "Constructor.hpp"
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
}
