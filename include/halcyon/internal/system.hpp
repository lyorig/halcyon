#pragma once

#include <halcyon/types/exception.hpp>
#include <halcyon/types/numeric.hpp>

#include <halcyon/utility/enum_bits.hpp>

#include <halcyon/misc.hpp>

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

    namespace proxy
    {
        class video;
        class audio;
        class events;
    }

    namespace detail
    {
        // Proxy dispatch.
        template <system S>
        struct prox_dsp;

        template <>
        struct prox_dsp<system::video>
        {
            using type = proxy::video;
        };

        template <>
        struct prox_dsp<system::audio>
        {
            using type = proxy::audio;
        };

        template <>
        struct prox_dsp<system::events>
        {
            using type = proxy::events;
        };

        template <system... Systems>
        class init_base : public prox_dsp<Systems>::type...
        {
        public:
            consteval static system_mask systems()
            {
                return { Systems... };
            }

        private:
            // Flag checks to ensure some unnecessary BS doesn't happen.
            static_assert(!systems().all({ system::video, system::events }), "Incompatible event types; Events is already initialized by Video.");

            static bool do_init()
            {
                return ::SDL_InitSubSystem(systems().mask()) == 0;
            }

        public:
            // Initialize chosen subsystems.
            // This constructor throws if initialization fails.
            init_base()
            {
                if (!do_init())
                    throw hal::exception {};
            }

            // Initialize chosen subsystems.
            // This constructor stores the initialization result in [succeeded].
            init_base(bool& succeeded)
            {
                succeeded = do_init();
            }

            // Initialize chosen subsystems.
            // This constructor does no error checking whatsoever.
            init_base(std::nothrow_t)
            {
                static_cast<void>(do_init());
            }

            init_base(const init_base&) = delete;
            init_base(init_base&&)      = delete;
        };
    }

    // Manages system (de)initialization via RAII.
    // Upon destruction, only the chosen subsystems are de-initialized;
    // hal::cleanup() should still be called at the end of your program.
    // See also hal::cleanup_init, which does this for you.
    template <system... Systems>
    class init : public detail::init_base<Systems...>
    {
    private:
        using super = detail::init_base<Systems...>;

    public:
        using super::init_base;

        ~init()
        {
            ::SDL_QuitSubSystem(super::systems().mask());
        }
    };

    // Initializes specific systems and calls hal::cleanup() upon destruction.
    template <system... Systems>
    class cleanup_init : public detail::init_base<Systems...>
    {
    public:
        using detail::init_base<Systems...>::init_base;

        ~cleanup_init()
        {
            hal::cleanup();
        }
    };
}
