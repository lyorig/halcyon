#include <halcyon/misc.hpp>

#include "SDL_misc.h"

using namespace hal;

outcome hal::open_url(c_string uri)
{
    return ::SDL_OpenURL(uri.c_str());
}
