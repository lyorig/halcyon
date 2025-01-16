#pragma once

#include <halcyon/types/c_string.hpp>

#include "SDL3/SDL_audio.h"

namespace hal
{
    namespace audio
    {
        using freq_t = std::uint32_t;

        // Formats in which audio data is stored.
        enum class format : std::uint16_t
        {
            i8 = AUDIO_S8,

            i16     = AUDIO_S16SYS,
            i16_lsb = AUDIO_S16LSB,
            i16_msb = AUDIO_S16MSB,

            i32     = AUDIO_S32SYS,
            i32_lsb = AUDIO_S32LSB,
            i32_msb = AUDIO_S32MSB,

            u8 = AUDIO_U8,

            u16     = AUDIO_U16SYS,
            u16_lsb = AUDIO_U16LSB,
            u16_msb = AUDIO_U16MSB,

            f32     = AUDIO_F32SYS,
            f32_lsb = AUDIO_F32LSB,
            f32_msb = AUDIO_F32MSB
        };

        // Which changes to allow if the desired audio spec cannot be met.
        enum class change : std::uint8_t
        {
            frequency = SDL_AUDIO_ALLOW_FREQUENCY_CHANGE,
            format    = SDL_AUDIO_ALLOW_FORMAT_CHANGE,
            channels  = SDL_AUDIO_ALLOW_CHANNELS_CHANGE,
            samples   = SDL_AUDIO_ALLOW_SAMPLES_CHANGE,
            any       = SDL_AUDIO_ALLOW_ANY_CHANGE
        };
    }

    constexpr c_string to_string(audio::format fmt)
    {
        using enum audio::format;

        switch (fmt)
        {
        case u8:
            return "u8";

        case i8:
            return "i8";

        case u16_lsb:
            return "u16 LSB";

        case u16_msb:
            return "u16 MSB";

        case i16_lsb:
            return "i16 LSB";

        case i16_msb:
            return "i16 MSB";

        case i32_lsb:
            return "i32 LSB";

        case i32_msb:
            return "i32 MSB";

        case f32_lsb:
            return "f32 LSB";

        case f32_msb:
            return "f32 MSB";

        default:
            return "[unknown]";
        }
    }

    constexpr c_string to_string(audio::change ch)
    {
        using enum audio::change;

        switch (ch)
        {
        case frequency:
            return "Frequency";

        case format:
            return "Format";

        case channels:
            return "Channels";

        case samples:
            return "Samples";

        case any:
            return "Any";

        default:
            return "[unknown]";
        }
    }
}
