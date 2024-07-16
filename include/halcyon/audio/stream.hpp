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
        class stream;
    }

    template <>
    class view<const audio::stream> : public detail::view_base<SDL_AudioStream>
    {
    protected:
        using view_base::view_base;

    public:
        i32 available() const;

        template <meta::buffer T>
        i32 get_processed(T& buffer) const
        {
            const int ret { ::SDL_AudioStreamGet(get(), std::data(buffer), std::size(buffer)) };

            HAL_ASSERT(ret != -1, debug::last_error());

            return ret;
        }
    };

    template <>
    class view<audio::stream> : public view<const audio::stream>
    {
    private:
        using super = view<const audio::stream>;

    protected:
        using super::super;

    public:
        void flush();

        void clear();

        void put(std::span<const std::byte> data);
    };

    namespace audio
    {
        struct config
        {
            format fmt;
            u8     channels;
            i32    rate;
        };

        // A stream that transforms an input of a certain format to an output of another format.
        class stream : public detail::raii_object<stream, ::SDL_FreeAudioStream>
        {
        public:
            // Default constructor. Creates an invalid stream.
            stream() = default;

            stream(proxy::audio& sys, config src, config dst);
        };
    }
}
