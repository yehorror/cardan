#include "ScriptExecutionContext.hpp"

namespace cardan
{
    ScriptExecutionContext::ScriptExecutionContext(const std::string& /*src*/)
    {
    }

    std::variant<std::string, int> ScriptExecutionContext::runScript()
    {
        return "";
    }
}
