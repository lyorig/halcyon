#include <halcyon/audio.hpp>

using namespace hal;

audio::device::id_t proxy::audio::outputs_size() const
{
    return static_cast<hal::audio::device::id_t>(::SDL_GetNumAudioDevices(false));
}

c_string proxy::audio::outputs_name(hal::audio::device::id_t idx) const
{
    return ::SDL_GetAudioDeviceName(idx, false);
}

result<hal::audio::spec> proxy::audio::outputs_spec(hal::audio::device::id_t idx) const
{
    hal::audio::spec ret;

    return { ::SDL_GetAudioDeviceSpec(idx, false, ret.get(pass_key<audio> {})), ret };
}

audio::device::id_t proxy::audio::inputs_size() const
{
    return static_cast<hal::audio::device::id_t>(::SDL_GetNumAudioDevices(true));
}

c_string proxy::audio::inputs_name(hal::audio::device::id_t idx) const
{
    return ::SDL_GetAudioDeviceName(idx, true);
}

result<hal::audio::spec> proxy::audio::inputs_spec(hal::audio::device::id_t idx) const
{
    hal::audio::spec ret;

    return { ::SDL_GetAudioDeviceSpec(idx, true, ret.get(pass_key<audio> {})), ret };
}

proxy::audio::audio()
{
}
