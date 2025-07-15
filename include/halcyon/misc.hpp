#pragma once

#include <halcyon/types/c_string.hpp>
#include <halcyon/types/result.hpp>

// misc.hpp:
// Miscellaneous definitions etc. that don't fit elsewhere.

#ifdef _MSC_VER
    #define HAL_NO_SIZE [[msvc::no_unique_address]]
#else
    #define HAL_NO_SIZE [[no_unique_address]]
#endif

namespace hal
{
    bool open_url(c_string url);
}
