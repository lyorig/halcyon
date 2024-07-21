#pragma once

#include <span>

#include <halcyon/audio/types.hpp>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/internal/subsystem.hpp>

#include <halcyon/utility/concepts.hpp>

namespace hal
{
    namespace audio
    {
        struct config
        {
            format fmt;
            u8     channels;
            i32    rate;
        };

        // A stream that transforms an input of a certain format to an output of another format.
        class stream : public detail::raii_object<SDL_AudioStream, ::SDL_FreeAudioStream>
        {
        public:
            // Default constructor. Creates an invalid stream.
            stream() = default;

            stream(proxy::audio& sys, config src, config dst);

            void flush();

            void clear();

            void put(std::span<const std::byte> data);

            i32 available() const;

            i32 get_processed(std::span<std::byte> buffer) const;
        };
    }
}
