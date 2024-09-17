#pragma once

#include <halcyon/types/exception.hpp>
#include <halcyon/types/numeric.hpp>

#include <halcyon/utility/enum_bits.hpp>

#include "SDL.h"

#include <new> // std::nothrow_t

// internal/system.hpp:
// A representation of SDL subsystems.

namespace hal
{
    enum class system : u16
    {
        video  = SDL_INIT_VIDEO,
        audio  = SDL_INIT_AUDIO,
        events = SDL_INIT_EVENTS
    };

    using system_mask = enum_bitmask<system>;

    // Check which systems have been initialized.
    system_mask initialized();

    // Check whether the specified system(s) have been initialized.
    bool initialized(system system);
    bool initialized(system_mask systems);

    // Ts... is a collection of proxies.
    template <typename... Ts>
    class init : public Ts...
    {
    private:
        constexpr static system_mask types { Ts::system... };

        // Flag checks to ensure some unnecessary BS doesn't happen.
        static_assert(!types.all({ system::events, system::video }), "Incompatible event types; Events is already initialized by Video.");

    public:
        init()
        {
            if (!do_init())
                throw hal::exception {};
        }

        init(std::nothrow_t)
        {
            static_cast<void>(do_init());
        }

        ~init()
        {
            ::SDL_QuitSubSystem(types.mask());
        }

        init(const init&) = delete;
        init(init&&)      = delete;

        static bool initialized()
        {
            return hal::initialized(types);
        }

    private:
        static bool do_init()
        {
            return ::SDL_InitSubSystem(types.mask()) == 0;
        }
    };
}
