#pragma once

namespace cardan
{
    class JSException : public std::exception
    {
    };

    class InvalidJSCodeException : public std::exception
    {
    };

}
