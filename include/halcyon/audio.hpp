#pragma once

#include <halcyon/misc.hpp>

#include <halcyon/audio/device.hpp>
#include <halcyon/audio/stream.hpp>

#include <halcyon/main.hpp>

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

    namespace proxy
    {
        class audio
        {
        public:
            constexpr static system system { system::audio };

            [[nodiscard]] hal::audio::builder::device build_device();

            [[nodiscard]] hal::audio::stream make_stream(hal::audio::config src, hal::audio::config dst);

            HAL_NO_SIZE proxy::audio_outputs outputs;
            HAL_NO_SIZE proxy::audio_inputs inputs;

        protected:
            audio();
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
