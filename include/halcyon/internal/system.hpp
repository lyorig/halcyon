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

        template <system T>
        using prox_dsp_t = prox_dsp<T>::type;
    }

    // Initializes systems and provides access, but doesn't deinitialize them upon
    // going out of scope. Useful for when you want to do a one-off deinitialization
    // with hal::cleanup().
    template <system... Systems>
    class leaky_init : public detail::prox_dsp_t<Systems>...
    {
    protected:
        constexpr static system_mask types { Systems... };

    private:
        // Flag checks to ensure some unnecessary BS doesn't happen.
        static_assert(!types.all({ system::video, system::events }), "Incompatible event types; Events is already initialized by Video.");

        static bool do_init()
        {
            return ::SDL_InitSubSystem(types.mask()) == 0;
        }

    public:
        leaky_init()
        {
            if (!do_init())
                throw hal::exception {};
        }

        leaky_init(std::nothrow_t)
        {
            static_cast<void>(do_init());
        }

        leaky_init(const leaky_init&) = delete;
        leaky_init(leaky_init&&)      = delete;
    };

    // Manages system (de)initialization.
    template <system... Systems>
    class init : public leaky_init<Systems...>
    {
    private:
        using super = leaky_init<Systems...>;

    public:
        using super::leaky_init;

        ~init()
        {
            ::SDL_QuitSubSystem(super::types.mask());
        }
    };
}
