#include <halcyon/events/keyboard.hpp>

using namespace hal;

keyboard::key keyboard::to_key(button btn)
{
    return static_cast<keyboard::key>(::SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(btn)));
}

keyboard::button keyboard::to_button(key k)
{
    return static_cast<keyboard::button>(::SDL_GetScancodeFromKey(static_cast<SDL_KeyCode>(k)));
}

keyboard::state_reference::state_reference(pass_key<authority_t>)
    : m_arr { ::SDL_GetKeyboardState(nullptr) }
{
}

bool keyboard::state_reference::operator[](button b) const
{
    return static_cast<bool>(m_arr[std::to_underlying(b)]);
}

bool keyboard::state_reference::operator[](key k) const
{
    return static_cast<bool>(m_arr[::SDL_GetScancodeFromKey(static_cast<SDL_KeyCode>(k))]);
}

keyboard::mod_state::mod_state(pass_key<authority_t>)
    : enum_bitmask { static_cast<mod>(::SDL_GetModState()) }
{
}

std::string_view hal::to_string(keyboard::button btn)
{
    return ::SDL_GetScancodeName(static_cast<SDL_Scancode>(btn));
}

std::string_view hal::to_string(keyboard::key k)
{
    return ::SDL_GetKeyName(static_cast<SDL_KeyCode>(k));
}
