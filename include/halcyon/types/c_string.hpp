#pragma once

#include <ostream>
#include <string>

namespace hal
{
    class c_string
    {
    public:
        constexpr c_string()
            : m_str { nullptr }
        {
        }

        constexpr c_string(const char* str)
            : m_str { str }
        {
        }

        constexpr c_string(const std::string& str)
            : c_string { str.c_str() }
        {
        }

        constexpr c_string(std::string_view str)
            : c_string { str.data() }
        {
        }

        constexpr c_string(std::nullptr_t) = delete;

        constexpr std::size_t length() const
        {
            if consteval
            {
                return std::char_traits<char>::length(data());
            }
            else
            {
                return std::strlen(m_str);
            }
        }

        constexpr char operator[](std::size_t pos) const
        {
            return m_str[pos];
        }

        constexpr const char* c_str() const
        {
            return m_str;
        }

        constexpr const char* begin() const
        {
            return m_str;
        }

        constexpr const char* end() const
        {
            return begin() + length();
        }

        constexpr const char* data() const
        {
            return begin();
        }

        constexpr operator std::string_view() const
        {
            return { m_str, length() };
        }

        constexpr friend std::ostream& operator<<(std::ostream& os, c_string str)
        {
            return os << str.c_str();
        }

    private:
        const char* m_str;
    };
}
