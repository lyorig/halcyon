#include <halcyon/audio.hpp>

using namespace hal;

proxy::audio_outputs::audio_outputs(pass_key<audio>)
{
}

audio::device::id_t proxy::audio_outputs::size() const
{
    return static_cast<hal::audio::device::id_t>(::SDL_GetNumAudioDevices(false));
}

std::string_view proxy::audio_outputs::name(hal::audio::device::id_t idx) const
{
    const char* ret { ::SDL_GetAudioDeviceName(idx, false) };

    HAL_ASSERT(ret != nullptr, debug::last_error());

    return ret;
}

hal::audio::sdl::spec proxy::audio_outputs::spec(hal::audio::device::id_t idx) const
{
    hal::audio::sdl::spec ret;

    HAL_ASSERT_VITAL(::SDL_GetAudioDeviceSpec(idx, false, ret.get(pass_key<audio_outputs> {})) == 0, debug::last_error());

    return ret;
}

proxy::audio_inputs::audio_inputs(pass_key<audio>)
{
}

audio::device::id_t proxy::audio_inputs::size() const
{
    return static_cast<hal::audio::device::id_t>(::SDL_GetNumAudioDevices(true));
}

std::string_view proxy::audio_inputs::name(hal::audio::device::id_t idx) const
{
    const char* ret { ::SDL_GetAudioDeviceName(idx, true) };

    HAL_ASSERT(ret != nullptr, debug::last_error());

    return ret;
}

hal::audio::sdl::spec proxy::audio_inputs::spec(hal::audio::device::id_t idx) const
{
    hal::audio::sdl::spec ret;

    HAL_ASSERT_VITAL(::SDL_GetAudioDeviceSpec(idx, true, ret.get(pass_key<audio_inputs> {})) == 0, debug::last_error());

    return ret;
}

proxy::audio::subsystem(pass_key<hal::system::audio>)
    : outputs { pass_key<proxy::audio> {} }
    , inputs { pass_key<proxy::audio> {} }
{
}

audio::builder::device proxy::audio::build_device() const&
{
    return *this;
}

audio::stream proxy::audio::make_stream(hal::audio::config src, hal::audio::config dst) const&
{
    return { *this, src, dst };
}
