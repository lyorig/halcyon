#pragma once

#include <halcyon/audio/spec.hpp>

#include <halcyon/internal/resource.hpp>

#include <halcyon/utility/enum_bits.hpp>
#include <halcyon/utility/pass_key.hpp>

#include <halcyon/types/c_string.hpp>
#include <halcyon/types/result.hpp>

#include <halcyon/subsystem.hpp>

#include <span>

namespace hal
{
    namespace proxy
    {
        class audio;
    }

    namespace audio
    {
        class device;

        namespace builder
        {
            class device
            {
            public:
                device(proxy::audio sys);

                // Choose a specific audio device.
                device& name(c_string name);

                // Have this be a capture device.
                device& capture(bool val = true);

                device& spec(const spec& s);

                device& changes(enum_bitmask<change> allowed_changes);

                audio::device operator()();
                audio::device operator()(class spec& obtained);

            private:
                class spec m_spec;
                c_string   m_name;
                int        m_allowedChanges;
                bool       m_capture;
            };
        }

        class device
        {
        public:
            using id_t = SDL_AudioDeviceID;

            // Device querying functions return -1 to signalize an unknown
            // amount of devices etc., but don't intend it to be an error.
            consteval static id_t invalid_id()
            {
                return -1;
            }

            // Default constructor. Creates an invalid device.
            device();

            // [private] Construct an audio device via hal::audio::builder::device.
            device(c_string name, bool capture, const SDL_AudioSpec* desired, SDL_AudioSpec* obtained, int allowed_changes, pass_key<builder::device>);
            ~device();

            device(const device&)            = delete;
            device& operator=(const device&) = delete;

            outcome queue(std::span<std::byte> bytes);

            void pause(bool p);

            void lock();
            void unlock();

            id_t id() const;

        private:
            id_t m_id;
        };
    }
}
