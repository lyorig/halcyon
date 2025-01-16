#pragma once

#include <halcyon/types/c_string.hpp>
#include <halcyon/types/result.hpp>

// video/driver.hpp:
// Video driver information and enumeration.

namespace hal
{
    class renderer_info;

    namespace driver
    {
        using index_t = std::uint8_t;

        constexpr index_t invalid_amount { 0 };

        // Get the amount of available video drivers.
        // Returns invalid_amount in case of failure.
        index_t amount();

        // Get the name of the current video driver.
        // Returns nullptr in case of failure.
        c_string name();

        // Get the name of the video driver at [idx].
        // Returns nullptr in case of failure.
        c_string name(index_t idx);

        // Get rendering info of the driver at [idx].
        // This does not provide the maximum texture size.
        outcome info(renderer_info& info, index_t idx);
    };
}
