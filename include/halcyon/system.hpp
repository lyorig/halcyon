#pragma once

#include <halcyon/types/string.hpp>

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

    // Get the name of the current platform.
    c_string platform();

    string base_path();

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

    namespace simd
    {
        void* malloc(std::size_t len);
        void* realloc(void* mem, std::size_t len);

        void free(void* mem);

        std::size_t alignment();

        template <typename T>
        class unique_ptr : public std::unique_ptr<T, meta::struct_functor<simd::free>>
        {
        public:
            unique_ptr() = default;

            unique_ptr(std::nullptr_t)
                : std::unique_ptr<T> { nullptr }
            {
            }

            unique_ptr(std::size_t len)
                : std::unique_ptr<T> { simd::malloc(len) }
            {
            }
        };
    }
}
