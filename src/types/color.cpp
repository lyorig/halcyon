#include <halcyon/types/color.hpp>

#include <halcyon/utility/printing.hpp>

#include <ostream>

std::ostream& hal::operator<<(std::ostream& str, color c)
{
    str << "[r" << to_printable_int(c.r)
        << " g" << to_printable_int(c.g)
        << " b" << to_printable_int(c.b)
        << " a" << to_printable_int(c.a)
        << ']';

    return str;
}
