#include <halcyon/halcyon.hpp>

#include <SDL.h>

const char* hal::platform()
{
    return ::SDL_GetPlatform();
}

void hal::cleanup()
{
    ::SDL_Quit();
}
