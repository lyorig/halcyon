#pragma once

#include <halcyon/audio/types.hpp>

#include <halcyon/internal/resource.hpp>
#include <halcyon/internal/system.hpp>

#include <span>

namespace hal
{
    namespace proxy
    {
        class audio;
    }

    namespace audio
    {
        struct config
        {
            format fmt;
            u8     channels;
            i32    rate;
        };

        // A stream that transforms an input of a certain format to an output of another format.
        class stream : public detail::resource<SDL_AudioStream, ::SDL_FreeAudioStream>
        {
        public:
            // Default constructor. Creates an invalid stream.
            stream() = default;

            stream(proxy::audio sys, config src, config dst);

            void flush();

            void clear();

            void put(std::span<const std::byte> data);

            i32 available() const;

            i32 get_processed(std::span<std::byte> buffer) const;
        };
    }
}
