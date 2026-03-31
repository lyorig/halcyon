#pragma once

#include <SDL3/SDL_hints.h>

namespace hal::hint
{
    const char* get(const char* name);
    void        set(const char* name, const char* value);
}
