#pragma once

#include <halcyon/types/numeric.hpp>

#include <halcyon/utility/enum_bits.hpp>

#include "SDL.h"

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
            { T::type() } -> std::same_as<type>;
        };

        template <proxy_check T>
        class init : public T
        {
        public:
            init()
            {
                ::SDL_InitSubSystem(static_cast<Uint32>(T::type()));
            }

            ~init()
            {
                ::SDL_QuitSubSystem(static_cast<Uint32>(T::type()));
            }

            init(const init&) = delete;
            init(init&&)      = delete;
        };
    }
}
