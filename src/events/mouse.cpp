#include <halcyon/events/mouse.hpp>

#include <utility>

#include <halcyon/debug.hpp>

using namespace hal;

mouse::state::state(pass_key<proxy::events>)
    : m_state { static_cast<std::uint8_t>(::SDL_GetMouseState(nullptr, nullptr)) }
{
}

mouse::state::state(Uint32 mask, pass_key<event::mouse_motion>)
    : m_state { static_cast<std::uint8_t>(mask) }
{
}

std::uint8_t mouse::state::mask() const
{
    return m_state;
}

bool mouse::state::operator[](button btn) const
{
    return static_cast<bool>(m_state & SDL_BUTTON(std::to_underlying(btn)));
}
