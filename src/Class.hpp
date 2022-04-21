#pragma once

#include <memory>
#include <string>
#include <functional>
#include <unordered_map>

namespace cardan
{
    struct MemberStorage
    {
        virtual ~MemberStorage() = default;
    };

    template <class ClassT>
    class Class
    {
    public:
        Class();

        template <typename ReturnType, typename... Args>
        void method(const std::string& name, ReturnType(ClassT::*methodRef)(Args...));

    private:
        std::unordered_map<std::string, std::unique_ptr<MemberStorage>> m_members;
    };
}
