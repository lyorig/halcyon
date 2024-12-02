#pragma once

// misc.hpp:
// Sort-of-important functions.

#include <halcyon/types/c_string.hpp>

#include "SDL_endian.h"

#ifdef _MSC_VER
    #define HAL_NO_SIZE [[msvc::no_unique_address]]
#else
    #define HAL_NO_SIZE [[no_unique_address]]
#endif

namespace hal
{
    enum class endian : bool
    {
        lil,
        big
    };

    namespace compile_settings
    {
        constexpr endian endianness {
#ifdef SDL_LIL_ENDIAN
            endian::lil
#elif defined SDL_BIG_ENDIAN
            endian::big
#else
    #error "No byte order specified by SDL"
#endif
        };
    }

    // Get the name of the current platform.
    c_string platform();

    // Force-unload all SDL libraries. Systems are destroyed automatically via RAII,
    // but it is nonetheless recommended to call this function at the end of your program,
    // for example by registering it with std::atexit(). Does not affect imaging and TTF!
    void cleanup();
}
