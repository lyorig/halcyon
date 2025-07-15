#pragma once

#include <halcyon/types/exception.hpp>

#include <halcyon/utility/enum_bits.hpp>

#include <halcyon/types/result.hpp>

#include "SDL3/SDL.h"

// subsystem.hpp:
// A representation of SDL subsystems.

namespace hal
{
    enum class subsystem : std::uint16_t
    {
        video  = SDL_INIT_VIDEO,
        events = SDL_INIT_EVENTS
    };

    using subsystem_mask = enum_bitmask<subsystem>;

    // Check which subsystems have been initialized.
    subsystem_mask initialized();

    // Check whether the specified subsystem(s) have been initialized.
    bool initialized(subsystem sub);
    bool initialized(subsystem_mask subs);

    // Unconditionally, forcefully shut down everything that's been initialized beforehand.
    // Usually, you won't need to use this yourself, as subsystems are managed via RAII.
    void cleanup();

    namespace proxy
    {
        class video;
        class events;
    }

    namespace detail
    {
        // Proxy dispatch.
        template <subsystem S>
        struct sub_to_proxy;

        template <>
        struct sub_to_proxy<subsystem::video>
        {
            using type = proxy::video;
        };

        template <>
        struct sub_to_proxy<subsystem::events>
        {
            using type = proxy::events;
        };

        template <subsystem... Subs>
        class init_base : public sub_to_proxy<Subs>::type...
        {
        public:
            consteval static subsystem_mask subsystems()
            {
                return { Subs... };
            }

        private:
            // Flag checks to ensure some unnecessary BS doesn't happen.
            static_assert(!subsystems().all({ subsystem::video, subsystem::events }), "Incompatible event types; Events is already initialized by Video.");

            static outcome sdl_init()
            {
                return ::SDL_InitSubSystem(subsystems().mask());
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
    template <subsystem... Subs>
    class init : public detail::init_base<Subs...>
    {
    private:
        using super = detail::init_base<Subs...>;

    public:
        using super::init_base;

        ~init()
        {
            ::SDL_QuitSubSystem(super::subsystems().mask());
        }
    };

    // Initializes specific systems and calls hal::cleanup() upon destruction.
    template <subsystem... Subs>
    class cleanup_init : public detail::init_base<Subs...>
    {
    public:
        using detail::init_base<Subs...>::init_base;

        ~cleanup_init()
        {
            hal::cleanup();
        }
    };
}
