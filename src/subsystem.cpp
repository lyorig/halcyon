#include <halcyon/subsystem.hpp>

using namespace hal;

subsystem_mask hal::initialized()
{
    return static_cast<subsystem>(::SDL_WasInit(0));
}

bool hal::initialized(subsystem sys)
{
    return ::SDL_WasInit(static_cast<Uint32>(sys)) != 0;
}

bool hal::initialized(subsystem_mask subs)
{
    return ::SDL_WasInit(subs.mask()) == subs.mask();
}

void hal::cleanup()
{
    ::SDL_Quit();
}
