#include <halcyon/other/keyboard_mouse.hpp>
#include <utility>

using namespace hal;

keyboard::state_reference::state_reference()
    : m_arr { ::SDL_GetKeyboardState(nullptr) }
{
}

bool keyboard::state_reference::operator[](button btn) const
{
    return m_arr[std::to_underlying(btn)];
}

bool keyboard::state_reference::operator[](key k) const
{
    return m_arr[::SDL_GetScancodeFromKey(static_cast<SDL_KeyCode>(k))];
}

mouse::state::state()
    : m_state { static_cast<lyo::u8>(::SDL_GetMouseState(nullptr, nullptr)) }
{
}

mouse::state::state(std::uint32_t mask, lyo::pass_key<events::mouse_motion>)
    : state { mask }
{
}

mouse::state::state(std::uint32_t mask, lyo::pass_key<events::mouse_button>)
    : state { mask }
{
}

mouse::state::state(std::uint32_t mask)
    : m_state { static_cast<lyo::u8>(mask) }
{
}

bool mouse::state::operator[](button btn) const
{
    return m_state & SDL_BUTTON(std::to_underlying(btn));
}

pixel_point mouse::pos()
{
    hal::point<int> ret;

    ::SDL_GetMouseState(&ret.x, &ret.y);

    return ret;
}