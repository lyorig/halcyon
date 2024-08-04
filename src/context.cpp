#include <halcyon/context.hpp>

#include <SDL.h>

using namespace hal;

std::string_view hal::platform()
{
    return ::SDL_GetPlatform();
}

context::~context()
{
    ::SDL_Quit();
}
