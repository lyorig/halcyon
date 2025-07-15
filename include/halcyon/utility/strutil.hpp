#pragma once

#include <array>
#include <charconv>
#include <sstream>

// utility/strutil.hpp:
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

    // Return [x] as a string contained in a null-terminated std::array, prefixed with 0x.
    template <std::integral T>
    constexpr auto int_to_hex_array(T x)
    {
        // 0x + hex(x) + null terminator.
        std::array<char, 2 + sizeof(T) * 2 + 1> ret { '0', 'x' };
        std::to_chars(ret.data() + 2, ret.data() + ret.size() - 1, x, 16);

        return ret;
    }
}
