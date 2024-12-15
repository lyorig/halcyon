#include <halcyon/system.hpp>

#include <halcyon/utility/metaprogramming.hpp>

#include "SDL_cpuinfo.h"

#include <string_view>

#include <ostream>

using namespace hal;

c_string hal::platform()
{
    return ::SDL_GetPlatform();
}

int hal::total_ram()
{
    return ::SDL_GetSystemRAM();
}

#define FUNC(name, sdl_name) \
    bool cpu::name() { return static_cast<bool>(::SDL_Has##sdl_name()); }

FUNC(avx, AVX)
FUNC(avx2, AVX2)
FUNC(avx512f, AVX512F)

FUNC(lsx, LSX)
FUNC(mmx, MMX)
FUNC(lasx, LASX)

FUNC(sse, SSE)
FUNC(sse2, SSE2)
FUNC(sse3, SSE3)
FUNC(sse4_1, SSE41)
FUNC(sse4_2, SSE42)

FUNC(neon, NEON)
FUNC(armsimd, ARMSIMD)

FUNC(_3dnow, 3DNow)
FUNC(rdtsc, RDTSC)
FUNC(altivec, AltiVec)

int cpu::logical_cores()
{
    return ::SDL_GetCPUCount();
}

int cpu::cache_line()
{
    return ::SDL_GetCPUCacheLineSize();
}

std::ostream& cpu::supported(std::ostream& str)
{
    struct
    {
        std::string_view name;
        func_ptr<bool>   func;
    } constexpr checks[] {
        { "AVX", avx },
        { "AVX2", avx2 },
        { "AVX512F", avx512f },
        { "LSX", lsx },
        { "MMX", mmx },
        { "LASX", lasx },
        { "SSE", sse },
        { "SSE2", sse2 },
        { "SSE3", sse3 },
        { "SSE4.1", sse4_1 },
        { "SSE4.2", sse4_2 },
        { "NEON", neon },
        { "ARM SIMD", armsimd },
        { "3DNow!", _3dnow },
        { "RDTSC", rdtsc },
        { "AltiVec", altivec },
    };

    str << '[';
    bool found { false };

    for (const auto& p : checks)
    {
        if (p.func())
        {
            if (found)
                str << ", ";

            str << p.name;
            found = true;
        }
    }

    str << ']';

    return str;
}

std::ostream& cpu::info(std::ostream& str)
{
    str << '['
        << logical_cores() << " logical cores, "
        << cache_line() << " B cache line, features: "
        << supported
        << ']';

    return str;
}
