#pragma once

#include <halcyon/types/exception.hpp>
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
        class guard : public T
        {
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
                ::SDL_QuitSubSystem(static_cast<Uint32>(T::type()));
            }

            guard(const guard&) = delete;
            guard(guard&&)      = delete;

        private:
            static bool init()
            {
                return ::SDL_InitSubSystem(static_cast<Uint32>(T::type())) == 0;
            }
        };
    }
}
