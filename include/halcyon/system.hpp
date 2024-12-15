#pragma once

#include <halcyon/types/c_string.hpp>

#include "SDL_endian.h"

#include <iosfwd>

// system.hpp:
// CPU feature set detection and system info.

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

    // How much RAM the system has, in MiB.
    int total_ram();

    // CPU features.
    namespace cpu
    {
        bool avx(), avx2(), avx512f();

        bool lsx(), mmx(), lasx();

        bool sse(), sse2(), sse3(), sse4_1(), sse4_2();

        bool neon(), armsimd();

        bool _3dnow(), rdtsc(), altivec();

        int logical_cores(), cache_line();

        // Output all supported CPU features.
        std::ostream& supported(std::ostream& str);

        std::ostream& info(std::ostream& str);
    }
}
