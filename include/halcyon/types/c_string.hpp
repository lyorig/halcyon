#pragma once

#include <ostream>
#include <string>

namespace hal
{
    class c_string
    {
    public:
        c_string()
            : m_str { nullptr }
        {
        }

        c_string(const char* str)
            : m_str { str }
        {
        }

        c_string(const std::string& str)
            : c_string { str.c_str() }
        {
        }

        c_string(std::string_view str)
            : c_string { str.data() }
        {
        }

        c_string(std::nullptr_t) = delete;

        std::size_t length() const
        {
            return std::strlen(m_str);
        }

        char operator[](std::size_t pos) const
        {
            return m_str[pos];
        }

        const char* c_str() const
        {
            return m_str;
        }

        const char* begin() const
        {
            return m_str;
        }

        const char* end() const
        {
            return begin() + length();
        }

        const char* data() const
        {
            return begin();
        }

        operator std::string_view() const
        {
            return { m_str, length() };
        }

        friend std::ostream& operator<<(std::ostream& os, c_string str)
        {
            return os << str.c_str();
        }

    private:
        const char* m_str;
    };
}
