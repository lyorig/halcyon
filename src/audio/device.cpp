#include <halcyon/audio/device.hpp>

#include <halcyon/utility/enum_bits.hpp>

using namespace hal;

using adb = audio::builder::device;

adb::device(proxy::audio&)
    : m_spec { 44100, format::i32, 2, 4096 }
    , m_name { nullptr }
    , m_allowedChanges { 0 }
    , m_capture { false }
{
}

adb& adb::name(std::string_view name)
{
    m_name = name.data();
    return *this;
}

adb& adb::capture(bool val)
{
    m_capture = val;
    return *this;
}

adb& adb::spec(const audio::spec& s)
{
    m_spec = s;
    return *this;
}

adb& adb::changes(enum_bitmask<change> allowed_changes)
{
    m_allowedChanges = allowed_changes.mask();
    return *this;
}

audio::device adb::operator()()
{
    return { m_name, m_capture, m_spec.get(pass_key<device> {}), nullptr, m_allowedChanges, pass_key<device> {} };
}

audio::device adb::operator()(sdl::spec& obtained)
{
    return { m_name, m_capture, m_spec.get(pass_key<device> {}), obtained.get(pass_key<device> {}), m_allowedChanges, pass_key<device> {} };
}

audio::device::device()
    : m_id { invalid_id }
{
}

audio::device::device(const char* name, bool capture, const SDL_AudioSpec* desired, SDL_AudioSpec* obtained, int allowed_changes, pass_key<builder::device>)
    : m_id { ::SDL_OpenAudioDevice(name, capture, desired, obtained, allowed_changes) }
{
    HAL_ASSERT(m_id != 0, debug::last_error());
}

audio::device::~device()
{
    ::SDL_CloseAudioDevice(m_id);
}

void audio::device::queue(std::span<std::byte> bytes)
{
    HAL_ASSERT_VITAL(::SDL_QueueAudio(m_id, bytes.data(), static_cast<Uint32>(bytes.size_bytes())) == 0, debug::last_error());
}

void audio::device::pause(bool p)
{
    ::SDL_PauseAudioDevice(m_id, p);
}

void audio::device::lock()
{
    ::SDL_LockAudioDevice(m_id);
}

void audio::device::unlock()
{
    ::SDL_UnlockAudioDevice(m_id);
}

audio::device::id_t audio::device::id() const
{
    return m_id;
}
