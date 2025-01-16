#pragma once

#include <halcyon/misc.hpp>

#include <halcyon/audio/device.hpp>
#include <halcyon/audio/stream.hpp>

#include <halcyon/main.hpp>

namespace hal
{
    namespace proxy
    {
        class audio
        {
        public:
            hal::audio::device::id_t outputs_size() const;
            c_string                 outputs_name(hal::audio::device::id_t idx) const;
            result<hal::audio::spec> outputs_spec(hal::audio::device::id_t idx) const;

            hal::audio::device::id_t inputs_size() const;
            c_string                 inputs_name(hal::audio::device::id_t idx) const;
            result<hal::audio::spec> inputs_spec(hal::audio::device::id_t idx) const;

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
