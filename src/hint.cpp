#include <halcyon/hint.hpp>

using namespace hal;

const char* hint::get(const char* name)
{
    return ::SDL_GetHint(name);
}

void hint::set(const char* name, const char* value)
{
    ::SDL_SetHint(name, value);
}
