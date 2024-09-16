#pragma once

#include <halcyon/utility/pass_key.hpp>

#include <halcyon/video/types.hpp>

#include "SDL_video.h"

// video/display.hpp:
// A representation of a monitor.

namespace hal
{
    namespace proxy
    {
        class display;
    }

    namespace display
    {
        using id_t = u8;
        using hz_t = hal::u16;
    }

    namespace info
    {
        namespace sdl
        {
            class display;
        }

        class display
        {
        public:
            display() = default;
            display(const sdl::display& src);

            pixel::point       size;
            pixel::format      fmt;
            hal::display::hz_t hz;
        };

        namespace sdl
        {
            // Display device data.
            class display : private SDL_DisplayMode
            {
            public:
                display() = default;
                display(hal::display::id_t id, pass_key<proxy::display>);
                display(const info::display& src);

                pixel::point       size() const;
                pixel::format      format() const;
                hal::display::hz_t hz() const;

                friend std::ostream& operator<<(std::ostream& str, const info::display& disp);
            };

            static_assert(sizeof(display) == sizeof(SDL_DisplayMode));
        }
    }
}