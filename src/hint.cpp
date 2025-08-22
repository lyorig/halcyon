#include <halcyon/hint.hpp>

using namespace hal;

c_string hint::get(c_string name)
{
    return ::SDL_GetHint(name.c_str());
}

void hint::set(c_string name, c_string value)
{
    ::SDL_SetHint(name.c_str(), value.c_str());
}
