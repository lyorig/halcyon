#include <halcyon/types/result.hpp>

#include <ostream>

std::ostream& hal::operator<<(std::ostream& str, outcome o)
{
    str << (o ? "success" : "failure");

    return str;
}
