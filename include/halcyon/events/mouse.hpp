#pragma once

#include <halcyon/subsystem.hpp>
#include <halcyon/utility/pass_key.hpp>
#include <halcyon/video/types.hpp>

#include <SDL3/SDL_mouse.h>

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

    constexpr std::string_view to_string(mouse::button btn)
    {
        using enum hal::mouse::button;
        using namespace std::string_view_literals;

        switch (btn)
        {
        case left:
            return "Left Mouse"sv;

        case right:
            return "Right Mouse"sv;

        case middle:
            return "Middle Mouse"sv;

        case x1:
            return "Mouse Extra 1"sv;

        case x2:
            return "Mouse Extra 2"sv;
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
