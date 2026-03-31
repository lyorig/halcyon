#pragma once

#include <halcyon/types/result.hpp>

// video/driver.hpp:
// Video driver information and enumeration.

namespace hal
{
    class renderer_info;

    namespace driver
    {
        using index_t = std::uint8_t;

        // Get the amount of available video drivers.
        index_t amount();

        // Get the name of the current video driver.
        // Returns an invalid string if none has been initialized.
        const char* name();

        // Get the name of the video driver at [idx].
        // Returns an invalid string in case of failure.
        const char* name(index_t idx);
    };
}
