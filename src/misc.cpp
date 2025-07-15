#include <halcyon/misc.hpp>

#include "SDL3/SDL_misc.h"

using namespace hal;

bool hal::open_url(c_string url)
{
    return ::SDL_OpenURL(url.c_str());
}
