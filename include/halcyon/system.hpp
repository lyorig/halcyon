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
            _3ds          = 1ull << 0,
            aix           = 1ull << 1,
            android       = 1ull << 2,
            apple         = 1ull << 3,
            bsdi          = 1ull << 4,
            cygwin        = 1ull << 5,
            emscripten    = 1ull << 6,
            freebsd       = 1ull << 7,
            gdk           = 1ull << 8,
            haiku         = 1ull << 9,
            hpux          = 1ull << 10,
            hurd          = 1ull << 11,
            ios           = 1ull << 12,
            irix          = 1ull << 13,
            linux_        = 1ull << 14, // Macro shadowing
            macos         = 1ull << 15,
            netbsd        = 1ull << 16,
            ngage         = 1ull << 17,
            openbsd       = 1ull << 18,
            os2           = 1ull << 19,
            osf           = 1ull << 20,
            ps2           = 1ull << 21,
            psp           = 1ull << 22,
            qnxnto        = 1ull << 23,
            riscos        = 1ull << 24,
            solaris       = 1ull << 25,
            tvos          = 1ull << 26,
            unix_         = 1ull << 27, // Macro shadowing
            visionos      = 1ull << 28,
            vita          = 1ull << 29,
            win32         = 1ull << 30,
            windows       = 1ull << 31,
            wingdk        = 1ull << 32,
            xbox_one      = 1ull << 33,
            xbox_series   = 1ull << 34,
            windows_phone = 1ull << 35
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

        // Convenience function to check whether you're
        // compiling for Windows. Mirrors `SDL_PLATFORM_WINDOWS`.
        constexpr bool is_windows()
        {
            return current[type::windows];
        }

        // Convenience function to check whether you're
        // compiling for Linux. Mirrors `SDL_PLATFORM_LINUX`.
        constexpr bool is_linux()
        {
            return current[type::linux_];
        }

        // Convenience function to check whether you're
        // compiling for macOS. Mirrors `SDL_PLATFORM_MACOS`.
        constexpr bool is_macos()
        {
            return current[type::macos];
        }

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
        bool altivec();

        int logical_cores(), cache_line();

        std::size_t simd_alignment();

        // Output all supported CPU features.
        std::ostream& supported(std::ostream& str);

        std::ostream& info(std::ostream& str);
    }
}
