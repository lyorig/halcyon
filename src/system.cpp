#include <SDL.h>
#include <halcyon/system.hpp>

using namespace hal;

system_mask hal::initialized()
{
    return static_cast<system>(::SDL_WasInit(0));
}

bool hal::initialized(system sys)
{
    return ::SDL_WasInit(static_cast<Uint32>(sys)) != 0;
}

bool hal::initialized(system_mask systems)
{
    return ::SDL_WasInit(systems.mask()) == systems.mask();
}
