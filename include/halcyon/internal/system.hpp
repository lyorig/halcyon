#pragma once

#include <halcyon/types/exception.hpp>
#include <halcyon/types/numeric.hpp>

#include <halcyon/utility/enum_bits.hpp>

#include "SDL.h"

#include <concepts> // std::same_as
#include <new>      // std::nothrow_t

// internal/system.hpp:
// A representation of SDL subsystems.

namespace hal
{
    namespace system
    {
        enum class type : u16
        {
            video  = SDL_INIT_VIDEO,
            audio  = SDL_INIT_AUDIO,
            events = SDL_INIT_EVENTS
        };

        using type_mask = enum_bitmask<type>;

        // Check whether the specified system(s) have been initialized.
        bool initialized(type system);
        bool initialized(type_mask systems);

        template <type T>
        class base
        {
        public:
            constexpr static type type()
            {
                return T;
            }
        };

        template <typename T>
        concept proxy_check = requires {
                                  {
                                      T::type()
                                  } -> std::same_as<type>;
                              };

        // Ts... is a collection of proxies.
        template <proxy_check... Ts>
        class guard : public Ts...
        {
        private:
            constexpr static type_mask types { Ts::type()... };

            // Flag checks to ensure some unnecessary BS doesn't happen.
            static_assert(!types.all({ type::events, type::video }), "Incompatible event types; Events is already initialized by Video.");

        public:
            guard()
            {
                if (!init())
                    throw hal::exception {};
            }

            guard(std::nothrow_t)
            {
                static_cast<void>(init());
            }

            ~guard()
            {
                ::SDL_QuitSubSystem(types.mask());
            }

            guard(const guard&) = delete;
            guard(guard&&)      = delete;

            static bool initialized()
            {
                return system::initialized(types);
            }

        private:
            static bool init()
            {
                return ::SDL_InitSubSystem(types.mask()) == 0;
            }
        };
    }
}
