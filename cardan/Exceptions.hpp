#pragma once

#include <stdexcept>

namespace cardan
{
    // TODO Add more information about Javascript exception here
    class JSException : public std::runtime_error
    {
    public:
        JSException(const std::string& message)
            : std::runtime_error(message)
        {
        }
    };

    // TODO Add more information about error: line, column, etc.
    class InvalidJSCodeException : public std::exception
    {
    };

}
