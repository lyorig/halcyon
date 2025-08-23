#pragma once

#include <halcyon/types/c_string.hpp>
#include <halcyon/types/result.hpp>

// misc.hpp:
// Miscellaneous definitions etc. that don't fit elsewhere.

namespace hal
{
    bool open_url(c_string url);
}
