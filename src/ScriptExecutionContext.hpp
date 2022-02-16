#pragma once

#include <string>
#include <variant>

#include <v8.h>
#include <libplatform/libplatform.h>

namespace cardan
{
    class ScriptExecutionContext
    {
    public:
        ScriptExecutionContext(const std::string& src);
        ~ScriptExecutionContext();

        std::variant<std::string, int> runScript();
    private:
        const std::string m_jsCode;
        std::unique_ptr<v8::ArrayBuffer::Allocator> m_arrayBuffer;
        v8::Isolate* m_isolate;
    };
}
