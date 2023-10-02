#pragma once

#include <lyo/bitset.hpp>

#include <halcyon/enums/buttons.hpp>
#include <halcyon/types/render.hpp>

namespace hal {
class engine;

class input_handler {
public:
    // The usual scancodes, plus mouse buttons.
    using key_storage = lyo::bitset<SDL_NUM_SCANCODES + 5, lyo::u64>;

    input_handler(engine& eng) noexcept;

    bool update() noexcept;

    bool pressed(hal::button btn) const noexcept;
    bool held(hal::button btn) const noexcept;
    bool released(hal::button btn) const noexcept;

private:
    // If the layout is ever modified, make sure the binder's "performance mode" is aware of it.
    key_storage m_pressed, m_held, m_released;

public:
    pixel_pos mouse;
};
} // namespace hal