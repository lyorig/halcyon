#include <halcyon/library.hpp>

using namespace hal;

library::library(const char* name)
    : resource { ::SDL_LoadObject(name) }
{
}
