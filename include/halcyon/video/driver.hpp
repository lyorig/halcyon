#pragma once

#include <halcyon/types/numeric.hpp>

#include <string_view>

// video/driver.hpp:
// Video driver information and enumeration.

namespace hal
{
    namespace info::sdl
    {
        class renderer;
    }

    namespace driver
    {
        using index_t = u8;

        // Get the amount of available video drivers.
        // Returns zero in case of failure.
        index_t amount();

        // Get the name of the current video driver.
        // Returns nullptr in case of failure.
        const char* name();

        // Get the name of the video driver at [idx].
        // Returns nullptr in case of failure.
        const char* name(index_t idx);

        // Get rendering info of the driver at [idx].
        // This does not provide the maximum texture size.
        info::sdl::renderer info(index_t idx);
    };
}
