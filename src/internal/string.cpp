#include <halcyon/internal/string.hpp>

#include <halcyon/utility/strutil.hpp>

#include <cstring>

using namespace hal;

string::string(char* ptr, pass_key<proxy::clipboard>)
    : resource { ptr }
{
}

string::pointer string::begin()
{
    return get();
}

string::const_pointer string::begin() const
{
    return get();
}

string::pointer string::end()
{
    return begin() + size();
}

string::const_pointer string::end() const
{
    return begin() + size();
}

std::size_t string::size() const
{
    return std::strlen(get());
}

string::pointer string::data()
{
    return get();
}

string::const_pointer string::data() const
{
    return get();
}

string::const_pointer string::c_str() const
{
    return get();
}

bool hal::operator==(string lhs, std::string_view rhs)
{
    return rhs == lhs.c_str();
}
