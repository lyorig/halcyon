#pragma once

#include <halcyon/debug.hpp>

// types/exception.hpp:
// This type exists purely to communicate failure.
// Additional info should be collected via debug::last_error().

namespace hal
{
    class exception
    {
    public:
        exception(std::string_view what)
            : m_what { what }
        {
        }

        std::string_view what() const
        {
            return m_what;
        }

        std::string with_error() const
        {
            std::string ret { m_what };
            ret += ": ";
            ret += debug::last_error();

            return ret;
        }

    private:
        std::string_view m_what;
    };
}
