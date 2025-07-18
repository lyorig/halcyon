#include <halcyon/library.hpp>

using namespace hal;

library::library(c_string name)
    : resource { ::SDL_LoadObject(name.c_str()) }
{
}
