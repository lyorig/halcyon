#include <halcyon/audio/stream.hpp>

using namespace hal;

audio::stream::stream(const proxy::audio&, config src, config dst)
    : resource { ::SDL_NewAudioStream(static_cast<SDL_AudioFormat>(src.fmt), src.channels, src.rate, static_cast<SDL_AudioFormat>(dst.fmt), dst.channels, dst.rate) }
{
}

void audio::stream::flush()
{
    HAL_ASSERT_VITAL(::SDL_AudioStreamFlush(get()) == 0, debug::last_error());
}

void audio::stream::clear()
{
    ::SDL_AudioStreamClear(get());
}

void audio::stream::put(std::span<const std::byte> data)
{
    HAL_ASSERT_VITAL(::SDL_AudioStreamPut(get(), data.data(), static_cast<int>(data.size_bytes())) == 0, debug::last_error());
}

i32 audio::stream::available() const
{
    return static_cast<bool>(::SDL_AudioStreamAvailable(get()));
}

i32 audio::stream::get_processed(std::span<std::byte> buffer) const
{
    const int ret { ::SDL_AudioStreamGet(get(), std::data(buffer), std::size(buffer)) };

    HAL_ASSERT(ret != -1, debug::last_error());

    return ret;
}
