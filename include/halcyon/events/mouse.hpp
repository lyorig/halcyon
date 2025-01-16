#pragma once

#include <halcyon/video/types.hpp>

#include <halcyon/utility/pass_key.hpp>

#include <halcyon/subsystem.hpp>

#include "SDL3/SDL_mouse.h"

// event/mouse.hpp:
// Mouse button access and data.

namespace hal
{
    namespace event
    {
        class mouse_motion;
    }

    namespace proxy
    {
        class events;
    }

    namespace mouse
    {
        enum class button : std::uint8_t
        {
            left   = SDL_BUTTON_LEFT,
            middle = SDL_BUTTON_MIDDLE,
            right  = SDL_BUTTON_RIGHT,
            x1     = SDL_BUTTON_X1,
            x2     = SDL_BUTTON_X2,
        };

        // A snapshot of the current mouse state. Unlike the keyboard state reference,
        // you need to recreate this object to observe any changes.
        class state
        {
        public:
            // [private] A snapshot of the current mouse state is provided by the event subsystem.
            state(pass_key<proxy::events>);

            // [private] Constructor meant for events.
            state(Uint32 mask, pass_key<event::mouse_motion>);

            std::uint8_t mask() const;

            bool operator[](button btn) const;

        private:
            std::uint8_t m_state;
        };

        pixel::point pos_rel();
        pixel::point pos_abs();
    }

    constexpr c_string to_string(mouse::button btn)
    {
        using enum hal::mouse::button;

        switch (btn)
        {
        case left:
            return "Left Mouse";

        case right:
            return "Right Mouse";

        case middle:
            return "Middle Mouse";

        case x1:
            return "Mouse Extra 1";

        case x2:
            return "Mouse Extra 2";
        }
    }

    namespace mouse
    {
        template <typename T>
        std::ostream& operator<<(std::ostream& str, const T& obj)
        {
            return str << hal::to_string(obj);
        }
    }
}
