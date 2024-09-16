#include <halcyon/internal/system.hpp>

using namespace hal;

bool system::initialized(type system)
{
    return ::SDL_WasInit(static_cast<Uint32>(system)) != 0;
}

bool system::initialized(type_mask systems)
{
    return ::SDL_WasInit(systems.mask()) == systems.mask();
}