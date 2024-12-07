#pragma once

#include <halcyon/types/exception.hpp>

#include <halcyon/utility/enum_bits.hpp>

#include <halcyon/misc.hpp>

#include <halcyon/types/result.hpp>

#include "SDL.h"

// system.hpp:
// A representation of SDL subsystems.

namespace hal
{
    enum class system : std::uint16_t
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
        struct system_to_proxy;

        template <>
        struct system_to_proxy<system::video>
        {
            using type = proxy::video;
        };

        template <>
        struct system_to_proxy<system::audio>
        {
            using type = proxy::audio;
        };

        template <>
        struct system_to_proxy<system::events>
        {
            using type = proxy::events;
        };

        template <system... Systems>
        class init_base : public system_to_proxy<Systems>::type...
        {
        public:
            consteval static system_mask systems()
            {
                return { Systems... };
            }

        private:
            // Flag checks to ensure some unnecessary BS doesn't happen.
            static_assert(!systems().all({ system::video, system::events }), "Incompatible event types; Events is already initialized by Video.");

            static outcome sdl_init()
            {
                return ::SDL_InitSubSystem(systems().mask());
            }

        public:
            // Initialize chosen subsystems.
            // Throws in case of failure.
            init_base()
            {
                if (!sdl_init())
                    throw exception { "system initialization" };
            }

            // Initialize chosen subsystems.
            // Writes success state to res.
            init_base(outcome& res)
            {
                res = sdl_init();
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
