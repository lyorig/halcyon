#include <halcyon/context.hpp>

#include <SDL.h>

using namespace hal;

const char* hal::platform()
{
    return ::SDL_GetPlatform();
}

context::~context()
{
    ::SDL_Quit();
}
