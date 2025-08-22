#pragma once

#include <halcyon/types/c_string.hpp>

#include <SDL3/SDL_hints.h>

namespace hal::hint
{
    c_string get(c_string name);
    void     set(c_string name, c_string value);
}
