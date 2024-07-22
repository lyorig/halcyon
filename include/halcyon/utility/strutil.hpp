#pragma once

#include <sstream>

// strutil.hpp:
// String utility functions.

namespace hal
{
    // String equality check.
    bool streq(const char* lhs, const char* rhs);
    bool streq(const wchar_t* lhs, const wchar_t* rhs);

    // Input all arguments into a stringstream and return them as a string.
    template <typename... Args>
    std::string string_from_pack(Args&&... args)
    {
        // Warning suppression.
        if constexpr (sizeof...(Args) == 0)
            return {};

        else
        {
            std::stringstream stream;

            (stream << ... << std::forward<Args>(args));

            return stream.str();
        }
    }
}
