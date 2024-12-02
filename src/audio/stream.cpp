#include <halcyon/audio/stream.hpp>

#include <halcyon/audio.hpp>

using namespace hal;

audio::stream::stream(proxy::audio, config src, config dst)
    : resource { ::SDL_NewAudioStream(static_cast<SDL_AudioFormat>(src.fmt), src.channels, src.rate, static_cast<SDL_AudioFormat>(dst.fmt), dst.channels, dst.rate) }
{
}

outcome audio::stream::flush()
{
    return ::SDL_AudioStreamFlush(get());
}

void audio::stream::clear()
{
    ::SDL_AudioStreamClear(get());
}

outcome audio::stream::put(std::span<const std::byte> data)
{
    return ::SDL_AudioStreamPut(get(), data.data(), static_cast<int>(data.size_bytes()));
}

std::int32_t audio::stream::available() const
{
    return static_cast<bool>(::SDL_AudioStreamAvailable(get()));
}

std::int32_t audio::stream::get_processed(std::span<std::byte> buffer) const
{
    return ::SDL_AudioStreamGet(get(), std::data(buffer), std::size(buffer));
}
