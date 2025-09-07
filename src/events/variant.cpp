#include <halcyon/events/variant.hpp>

#include <halcyon/debug.hpp>

using namespace hal;

// Display event.

display::id_t event::display::display_index() const
{
    return static_cast<hal::display::id_t>(displayID);
}

event::display& event::display::display_index(hal::display::id_t disp)
{
    displayID = disp;

    return *this;
}

// Window event.

window::id_t event::window::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

event::window& event::window::window_id(hal::window::id_t id)
{
    windowID = id;

    return *this;
}

display::id_t event::window::display_index() const
{
    return static_cast<hal::display::id_t>(data1);
}

event::window& event::window::display_index(hal::display::id_t id)
{
    data1 = id;

    return *this;
}

pixel::point event::window::point() const
{
    return { static_cast<pixel_t>(data1), static_cast<pixel_t>(data2) };
}

event::window& event::window::point(pixel::point pt)
{
    data1 = pt.x;
    data2 = pt.y;

    return *this;
}

// Keyboard event.

window::id_t event::keyboard::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

event::keyboard& event::keyboard::window_id(hal::window::id_t id)
{
    windowID = id;

    return *this;
}

keyboard::button event::keyboard::button() const
{
    return static_cast<hal::keyboard::button>(scancode);
}

event::keyboard& event::keyboard::button(hal::keyboard::button btn)
{
    scancode = static_cast<SDL_Scancode>(btn);

    return *this;
}

keyboard::key event::keyboard::key() const
{
    return static_cast<hal::keyboard::key>(SDL_KeyboardEvent::key);
}

event::keyboard& event::keyboard::key(hal::keyboard::key k)
{
    SDL_KeyboardEvent::key = static_cast<SDL_Keycode>(k);

    return *this;
}

bool event::keyboard::repeat() const
{
    return static_cast<bool>(SDL_KeyboardEvent::repeat);
}

event::keyboard& event::keyboard::repeat(bool r)
{
    SDL_KeyboardEvent::repeat = r;

    return *this;
}

// Mouse motion event.

window::id_t event::mouse_motion::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

event::mouse_motion& event::mouse_motion::window_id(hal::window::id_t id)
{
    windowID = id;

    return *this;
}

mouse::state event::mouse_motion::state() const
{
    return { SDL_MouseMotionEvent::state, pass_key<mouse_motion> {} };
}

event::mouse_motion& event::mouse_motion::state(mouse::state s)
{
    SDL_MouseMotionEvent::state = s.mask();

    return *this;
}

pixel::point event::mouse_motion::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

event::mouse_motion& event::mouse_motion::pos(pixel::point p)
{
    x = p.x;
    y = p.y;

    return *this;
}

pixel::point event::mouse_motion::rel() const
{
    return { static_cast<pixel_t>(xrel), static_cast<pixel_t>(yrel) };
}

event::mouse_motion& event::mouse_motion::rel(pixel::point p)
{
    xrel = p.x;
    yrel = p.y;

    return *this;
}

// Mouse button event.

window::id_t event::mouse_button::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

event::mouse_button& event::mouse_button::window_id(hal::window::id_t id)
{
    windowID = id;

    return *this;
}

mouse::button event::mouse_button::button() const
{
    return static_cast<mouse::button>(SDL_MouseButtonEvent::button);
}

event::mouse_button& event::mouse_button::button(mouse::button btn)
{
    SDL_MouseButtonEvent::button = static_cast<Uint8>(btn);

    return *this;
}

std::uint8_t event::mouse_button::click_amount() const
{
    return static_cast<std::uint8_t>(clicks);
}

event::mouse_button& event::mouse_button::click_amount(std::uint8_t amnt)
{
    clicks = amnt;

    return *this;
}

pixel::point event::mouse_button::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

event::mouse_button& event::mouse_button::pos(pixel::point pt)
{
    x = pt.x;
    y = pt.y;

    return *this;
}

// Mouse wheel event.

window::id_t event::mouse_wheel::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

event::mouse_wheel& event::mouse_wheel::window_id(hal::window::id_t id)
{
    windowID = id;

    return *this;
}

pixel::point event::mouse_wheel::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

event::mouse_wheel& event::mouse_wheel::pos(pixel::point p)
{
    x = p.x;
    y = p.y;

    return *this;
}

point<std::int16_t> event::mouse_wheel::scroll() const
{
    return {
        static_cast<std::int16_t>(x),
        static_cast<std::int16_t>(y)
    };
}

event::mouse_wheel& event::mouse_wheel::scroll(point<std::int16_t> s)
{
    x = s.x;
    y = s.y;

    return *this;
}

point<float> event::mouse_wheel::scroll_precise() const
{
    return { x, y };
}

event::mouse_wheel& event::mouse_wheel::scroll_precise(point<float> s)
{
    x = s.x;
    y = s.y;

    return *this;
}

bool event::mouse_wheel::scroll_flipped() const
{
    return direction == SDL_MOUSEWHEEL_FLIPPED;
}

event::mouse_wheel& event::mouse_wheel::scroll_flipped(bool f)
{
    static_assert(SDL_MOUSEWHEEL_NORMAL == false && SDL_MOUSEWHEEL_FLIPPED == true);

    direction = static_cast<SDL_MouseWheelDirection>(f);

    return *this;
}

// Text editing event.

window::id_t event::text_input::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

event::text_input& event::text_input::window_id(hal::window::id_t id)
{
    windowID = id;

    return *this;
}

c_string event::text_input::text() const
{
    return SDL_TextInputEvent::text;
}

event::text_input& event::text_input::text(c_string t)
{
    SDL_TextInputEvent::text = t.c_str();

    return *this;
}

// Event handler.

event::variant::variant()
    : m_event { .type = std::numeric_limits<std::uint32_t>::max() } // Start with an invalid event.
{
}

event::type event::variant::kind() const
{
    return static_cast<type>(m_event.type);
}

void event::variant::kind(event::type t)
{
    m_event.type = std::to_underlying(t);
}

const event::display& event::variant::display() const
{
    return reinterpret_cast<const event::display&>(m_event.display);
}

event::display& event::variant::display()
{
    return reinterpret_cast<event::display&>(m_event.display);
}

const event::window& event::variant::window() const
{
    return reinterpret_cast<const event::window&>(m_event.window);
}

event::window& event::variant::window()
{
    return reinterpret_cast<event::window&>(m_event.window);
}

const event::keyboard& event::variant::keyboard() const
{
    HAL_ASSERT(kind() == type::key_pressed || kind() == type::key_released, "Invalid type");

    return reinterpret_cast<const event::keyboard&>(m_event.key);
}

event::keyboard& event::variant::keyboard()
{
    HAL_ASSERT(kind() == type::key_pressed || kind() == type::key_released, "Invalid type");

    return reinterpret_cast<event::keyboard&>(m_event.key);
}

const event::text_input& event::variant::text_input() const
{
    HAL_ASSERT(kind() == type::text_input, "Invalid type");

    return reinterpret_cast<const event::text_input&>(m_event.text);
}

event::text_input& event::variant::text_input()
{
    HAL_ASSERT(kind() == type::text_input, "Invalid type");

    return reinterpret_cast<event::text_input&>(m_event.text);
}

const event::mouse_motion& event::variant::mouse_motion() const
{
    HAL_ASSERT(kind() == type::mouse_moved, "Invalid type");

    return reinterpret_cast<const event::mouse_motion&>(m_event.motion);
}

event::mouse_motion& event::variant::mouse_motion()
{
    HAL_ASSERT(kind() == type::mouse_moved, "Invalid type");

    return reinterpret_cast<event::mouse_motion&>(m_event.motion);
}

const event::mouse_button& event::variant::mouse_button() const
{
    HAL_ASSERT(kind() == type::mouse_pressed || kind() == type::mouse_released, "Invalid type");

    return reinterpret_cast<const event::mouse_button&>(m_event.button);
}

event::mouse_button& event::variant::mouse_button()
{
    HAL_ASSERT(kind() == type::mouse_pressed || kind() == type::mouse_released, "Invalid type");

    return reinterpret_cast<event::mouse_button&>(m_event.button);
}

const event::mouse_wheel& event::variant::mouse_wheel() const
{
    HAL_ASSERT(kind() == type::mouse_wheel_moved, "Invalid type");

    return reinterpret_cast<const event::mouse_wheel&>(m_event.wheel);
}

event::mouse_wheel& event::variant::mouse_wheel()
{
    HAL_ASSERT(kind() == type::mouse_wheel_moved, "Invalid type");

    return reinterpret_cast<event::mouse_wheel&>(m_event.wheel);
}

bool event::variant::pending() const
{
    return ::SDL_PollEvent(nullptr) == 1;
}

const SDL_Event& event::variant::get(pass_key<proxy::events>) const
{
    return m_event;
}

SDL_Event& event::variant::get(pass_key<proxy::events>)
{
    return m_event;
}
