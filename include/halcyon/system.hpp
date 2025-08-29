#pragma once

#include <halcyon/types/string.hpp>
#include <halcyon/utility/enum_bits.hpp>

#include "SDL3/SDL_endian.h"
#include "SDL3/SDL_power.h"

#include <iosfwd>

// system.hpp:
// Detection of CPU features, various system parameters and SIMD operations.

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

    struct power_state
    {
        enum class battery_state : std::uint8_t
        {
            unknown    = SDL_POWERSTATE_UNKNOWN,
            on_battery = SDL_POWERSTATE_ON_BATTERY,
            no_battery = SDL_POWERSTATE_NO_BATTERY,
            charging   = SDL_POWERSTATE_CHARGING,
            charged    = SDL_POWERSTATE_CHARGED
        };

        enum : int
        {
            unknown_seconds = -1
        };

        enum : std::uint8_t
        {
            unknown_percent = static_cast<std::uint8_t>(-1)
        };

        // Returns a new, freshly updated power state.
        static power_state current();

        // Create a new uninitialized power state.
        power_state();

        // Update this structure in place.
        power_state& update();

        friend std::ostream& operator<<(std::ostream& str, power_state s);

        int           seconds;
        std::uint8_t  percent;
        battery_state battery;
    };

    constexpr std::string_view to_string(power_state::battery_state bs)
    {
        using enum power_state::battery_state;

        switch (bs)
        {
        case unknown:
            return "unknown";

        case on_battery:
            return "on battery";

        case no_battery:
            return "no battery";

        case charging:
            return "charging";

        case charged:
            return "charged";

        default:
            return "[unknown]";
        }
    }

    namespace platform
    {
        // Since multiple platforms can be defined at once,
        // this enum acts as a bitmask.
        enum class type : std::uint64_t
        {
            _3ds          = 1L << 0,
            aix           = 1L << 1,
            android       = 1L << 2,
            apple         = 1L << 3,
            bsdi          = 1L << 4,
            cygwin        = 1L << 5,
            emscripten    = 1L << 6,
            freebsd       = 1L << 7,
            gdk           = 1L << 8,
            haiku         = 1L << 9,
            hpux          = 1L << 10,
            hurd          = 1L << 11,
            ios           = 1L << 12,
            irix          = 1L << 13,
            linux_        = 1L << 14, // Macro shadowing
            macos         = 1L << 15,
            netbsd        = 1L << 16,
            ngage         = 1L << 17,
            openbsd       = 1L << 18,
            os2           = 1L << 19,
            osf           = 1L << 20,
            ps2           = 1L << 21,
            psp           = 1L << 22,
            qnxnto        = 1L << 23,
            riscos        = 1L << 24,
            solaris       = 1L << 25,
            tvos          = 1L << 26,
            unix_         = 1L << 27, // Macro shadowing
            visionos      = 1L << 28,
            vita          = 1L << 29,
            win32         = 1L << 30,
            windows       = 1L << 31,
            wingdk        = 1L << 32,
            xbox_one      = 1L << 33,
            xbox_series   = 1L << 34,
            windows_phone = 1L << 35
        };

        constexpr enum_bitmask<type> current {
#if SDL_PLATFORM_3DS
            type::_3ds,
#endif
#if SDL_PLATFORM_AIX
            type::aix,
#endif
#if SDL_PLATFORM_ANDROID
            type::android,
#endif
#if SDL_PLATFORM_APPLE
            type::apple,
#endif
#if SDL_PLATFORM_BSDI
            type::bsdi,
#endif
#if SDL_PLATFORM_CYGWIN
            type::cygwin,
#endif
#if SDL_PLATFORM_EMSCRIPTEN
            type::emscripten,
#endif
#if SDL_PLATFORM_FREEBSD
            type::freebsd,
#endif
#if SDL_PLATFORM_GDK
            type::gdk,
#endif
#if SDL_PLATFORM_HAIKU
            type::haiku,
#endif
#if SDL_PLATFORM_HPUX
            type::hpux,
#endif
#if SDL_PLATFORM_HURD
            type::hurd,
#endif
#if SDL_PLATFORM_IOS
            type::ios,
#endif
#if SDL_PLATFORM_IRIX
            type::irix,
#endif
#if SDL_PLATFORM_LINUX
            type::linux_,
#endif
#if SDL_PLATFORM_MACOS
            type::macos,
#endif
#if SDL_PLATFORM_NETBSD
            type::netbsd,
#endif
#if SDL_PLATFORM_NGAGE
            type::ngage,
#endif
#if SDL_PLATFORM_OPENBSD
            type::openbsd,
#endif
#if SDL_PLATFORM_OS2
            type::os2,
#endif
#if SDL_PLATFORM_OSF
            type::osf,
#endif
#if SDL_PLATFORM_PS2
            type::ps2,
#endif
#if SDL_PLATFORM_PSP
            type::psp,
#endif
#if SDL_PLATFORM_QNXNTO
            type::qnxnto,
#endif
#if SDL_PLATFORM_RISCOS
            type::riscos,
#endif
#if SDL_PLATFORM_SOLARIS
            type::solaris,
#endif
#if SDL_PLATFORM_TVOS
            type::tvos,
#endif
#if SDL_PLATFORM_UNIX
            type::unix_,
#endif
#if SDL_PLATFORM_VISIONOS
            type::visionos,
#endif
#if SDL_PLATFORM_VITA
            type::vita,
#endif
#if SDL_PLATFORM_WIN32
            type::win32,
#endif
#if SDL_PLATFORM_WINDOWS
            type::windows,
#endif
#if SDL_PLATFORM_WINGDK
            type::wingdk,
#endif
#if SDL_PLATFORM_XBOXONE
            type::xbox_one,
#endif
#if SDL_PLATFORM_XBOXSERIES
            type::xbox_series,
#endif
#if SDL_WINAPI_FAMILY_PHONE
            type::windows_phone,
#endif
        };

        c_string name();
    }

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

        std::size_t simd_alignment();

        // Output all supported CPU features.
        std::ostream& supported(std::ostream& str);

        std::ostream& info(std::ostream& str);
    }
}
