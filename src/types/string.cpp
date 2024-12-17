#include <halcyon/types/string.hpp>

#include <halcyon/utility/strutil.hpp>

#include <cstring>

using namespace hal;

string::string(char* str)
    : resource { str }
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

bool hal::operator==(string lhs, const char* rhs)
{
    return std::strcmp(lhs.c_str(), rhs) == 0;
}

std::ostream& hal::operator<<(std::ostream& str, const string& s)
{
    str << s.c_str();

    return str;
}
