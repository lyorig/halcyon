#include <halcyon/audio.hpp>

#include <iostream>

#include <halcyon/utility/printing.hpp>

using namespace hal;

// I am very mature.
using ass = audio::spec;

ass::spec(freq_t samples_per_second, audio::format fmt, std::uint8_t channels, std::uint16_t buffer_size_in_frames)
    : SDL_AudioSpec {
        .freq     = static_cast<int>(samples_per_second),
        .format   = static_cast<SDL_AudioFormat>(fmt),
        .channels = channels,
        .silence  = 0,
        .samples  = static_cast<Uint16>(buffer_size_in_frames),
        .padding  = 0,
        .size     = 0,
        .callback = nullptr,
        .userdata = nullptr
    }
{
}

audio::freq_t ass::hz() const
{
    return static_cast<freq_t>(freq);
}

audio::format ass::format() const
{
    return static_cast<audio::format>(SDL_AudioSpec::format);
}

std::uint8_t ass::channels() const
{
    return static_cast<std::uint8_t>(SDL_AudioSpec::channels);
}

std::uint16_t ass::buffer_size() const
{
    return samples;
}

SDL_AudioSpec* ass::get(pass_key<builder::device>)
{
    return this;
}

SDL_AudioSpec* ass::get(pass_key<proxy::audio>)
{
    return this;
}

std::ostream& audio::operator<<(std::ostream& str, const spec& s)
{
    return str << '[' << s.hz()
               << " Hz, format: "
               << s.format()
               << ", channels: "
               << to_printable_int(s.channels())
               << ", buffer size: "
               << s.buffer_size() << ']';
}
