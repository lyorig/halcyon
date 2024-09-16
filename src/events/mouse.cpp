#include <halcyon/events/mouse.hpp>

#include <utility>

#include <halcyon/debug.hpp>

using namespace hal;

mouse::state::state(pass_key<proxy::mouse>)
    : m_state { static_cast<u8>(::SDL_GetMouseState(nullptr, nullptr)) }
{
}

mouse::state::state(Uint32 mask, pass_key<event::mouse_motion>)
    : m_state { static_cast<u8>(mask) }
{
}

u8 mouse::state::mask() const
{
    return m_state;
}

bool mouse::state::operator[](button btn) const
{
    return static_cast<bool>(m_state & SDL_BUTTON(std::to_underlying(btn)));
}