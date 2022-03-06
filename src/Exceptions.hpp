#pragma once

#include <stdexcept>

namespace cardan
{
    // TODO Add more information about Javascript exception here
    class JSException : public std::exception
    {
    };

    // TODO Add more information about error: line, column, etc.
    class InvalidJSCodeException : public std::exception
    {
    };

}
