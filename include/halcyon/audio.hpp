#pragma once

#include <halcyon/context.hpp>

#include <halcyon/audio/device.hpp>
#include <halcyon/audio/stream.hpp>

#include <halcyon/internal/subsystem.hpp>

namespace hal
{
    namespace proxy
    {
        class audio_outputs
        {
        public:
            audio_outputs(pass_key<audio>);

            hal::audio::device::id_t size() const;
            std::string_view         name(hal::audio::device::id_t idx) const;
            hal::audio::sdl::spec    spec(hal::audio::device::id_t idx) const;
        };

        class audio_inputs
        {
        public:
            audio_inputs(pass_key<audio>);

            hal::audio::device::id_t size() const;
            std::string_view         name(hal::audio::device::id_t idx) const;
            hal::audio::sdl::spec    spec(hal::audio::device::id_t idx) const;
        };
    }

    namespace detail
    {
        template <>
        class subsystem<system::audio>
        {
        public:
            subsystem(pass_key<hal::system::audio>);

            [[nodiscard]] audio::builder::device build_device() const&;
            [[nodiscard]] audio::builder::device build_device() const&& = delete;

            [[nodiscard]] audio::stream make_stream(audio::config src, audio::config dst) const&;
            [[nodiscard]] audio::stream make_stream(audio::config src, audio::config dst) const&& = delete;

            HAL_NO_SIZE proxy::audio_outputs outputs;
            HAL_NO_SIZE proxy::audio_inputs inputs;
        };
    }

    namespace audio
    {
        template <typename T>
            requires requires(const T& x) { hal::to_string(x); }
        std::ostream& operator<<(std::ostream& str, const T& obj)
        {
            return str << hal::to_string(obj);
        }
    }
}
