#pragma once

#include <sstream>

// strutil.hpp:
// String utility functions.

namespace hal
{
    template <typename CharT>
    constexpr std::size_t strlen(const CharT* str)
    {
        return std::char_traits<CharT>::length(str);
    }

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
