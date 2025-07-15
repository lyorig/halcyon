#include <SDL3/SDL_cpuinfo.h>
#include <halcyon/system.hpp>

#include <halcyon/utility/metaprogramming.hpp>
#include <halcyon/utility/printing.hpp>

#include "SDL3/SDL_cpuinfo.h"
#include "SDL3/SDL_filesystem.h"
#include "SDL3/SDL_platform.h"

#include <string_view>

#include <ostream>

using namespace hal;

power_state power_state::current()
{
    power_state ret;

    ret.update();

    return ret;
}

power_state::power_state()
    : seconds { unknown_seconds }
    , percent { unknown_percent }
    , battery { battery_state::unknown }
{
}

power_state& power_state::update()
{
    int percent_conv;

    battery = static_cast<battery_state>(::SDL_GetPowerInfo(&seconds, &percent_conv));
    percent = static_cast<std::uint8_t>(percent_conv);

    return *this;
}

std::ostream& hal::operator<<(std::ostream& str, power_state s)
{
    str << "[state: " << to_string(s.battery) << ", ";

    if (s.percent == power_state::unknown_percent)
        str << "percentage unknown";
    else
        str << to_printable_int(s.percent) << " % charge";

    str << ", ";

    if (s.seconds == power_state::unknown_seconds)
        str << "seconds unknown";
    else
        str << to_printable_int(s.seconds) << " s left";

    str << ']';

    return str;
}

c_string hal::platform()
{
    return ::SDL_GetPlatform();
}

c_string hal::base_path()
{
    return ::SDL_GetBasePath();
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

FUNC(altivec, AltiVec)

int cpu::logical_cores()
{
    return ::SDL_GetNumLogicalCPUCores();
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

std::size_t cpu::simd_alignment()
{
    return ::SDL_GetSIMDAlignment();
}
