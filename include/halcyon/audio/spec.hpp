#pragma once

#include <halcyon/audio/types.hpp>

#include <halcyon/utility/pass_key.hpp>

#include <iosfwd>

namespace hal
{
    namespace proxy
    {
        class audio;
    }

    namespace audio
    {
        class spec;

        namespace builder
        {
            class device;
        }

        class spec : private SDL_AudioSpec
        {
        public:
            spec() = default;

            // Create your own spec.
            spec(freq_t samples_per_second, audio::format fmt, std::uint8_t channels, std::uint16_t buffer_size_in_frames);

            freq_t        hz() const;
            audio::format format() const;
            std::uint8_t  channels() const;
            std::uint16_t buffer_size() const;

            SDL_AudioSpec* get(pass_key<builder::device>);
            SDL_AudioSpec* get(pass_key<proxy::audio>);

            friend std::ostream& operator<<(std::ostream& str, const spec& s);
        };

        static_assert(sizeof(spec) == sizeof(SDL_AudioSpec));
    }
}
