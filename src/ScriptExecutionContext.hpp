#pragma once

#include <string>
#include <variant>

namespace cardan
{
    class ScriptExecutionContext
    {
    public:
        ScriptExecutionContext(const std::string& src);

        std::variant<std::string, int> runScript();
    };
}
