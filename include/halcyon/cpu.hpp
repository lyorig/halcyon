#pragma once

#include <iosfwd>

// cpu.hpp:
// CPU feature set detection, plus other goodies.

namespace hal
{
    // CPU features.
    namespace cpu
    {
        bool avx(), avx2(), avx512f();

        bool lsx(), mmx(), lasx();

        bool sse(), sse2(), sse3(), sse4_1(), sse4_2();

        bool neon(), armsimd();

        bool _3dnow(), rdtsc(), altivec();

        int logical(), cache_line();

        // Output all supported CPU features.
        std::ostream& supported(std::ostream& str);

        std::ostream& info(std::ostream& str);
    }

    // How much RAM the system has, in MiB.
    int total_ram();
}
