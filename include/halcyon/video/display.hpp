#pragma once

#include <halcyon/video/types.hpp>

#include <halcyon/utility/pass_key.hpp>

#include "SDL_video.h"

// video/display.hpp:
// A representation of a monitor.

namespace hal
{
    namespace proxy
    {
        class video;
    }

    // Display device data.
    class display : private SDL_DisplayMode
    {
    public:
        using id_t = u8;
        using hz_t = u16;

        display() = default;

        pixel::point  size() const;
        pixel::format format() const;
        hz_t          hz() const;

        SDL_DisplayMode* ptr(pass_key<proxy::video>);

        friend std::ostream& operator<<(std::ostream& str, const display& disp);
    };
}
