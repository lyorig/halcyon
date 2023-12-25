#pragma once

#include <SDL2/SDL.h>

#include <lyo/types.hpp>

// subsystem.hpp:
// A RAII way of initializing SDL subsystems.

namespace hal
{
    enum class subsys : lyo::u16
    {
        video = SDL_INIT_VIDEO,
        audio = SDL_INIT_AUDIO,
        controller = SDL_INIT_GAMECONTROLLER,
        haptic = SDL_INIT_HAPTIC,
        sensor = SDL_INIT_SENSOR,
        all = SDL_INIT_EVERYTHING
    };

    template <subsys System>
    class subsystem
    {
    public:
        subsystem()
        {
            ::SDL_InitSubSystem(Uint32(System));
        }

        ~subsystem()
        {
            ::SDL_QuitSubSystem(Uint32(System));
        }
    };
} // namespace hal