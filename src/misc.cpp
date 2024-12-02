#include <halcyon/misc.hpp>

#include "SDL.h"

using namespace hal;

c_string hal::platform()
{
    return ::SDL_GetPlatform();
}

void hal::cleanup()
{
    ::SDL_Quit();
}
