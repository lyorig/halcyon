#pragma once

#include <halcyon/events/keyboard.hpp>
#include <halcyon/events/mouse.hpp>

#include <halcyon/video/display.hpp>
#include <halcyon/video/window.hpp>

// events/variant.hpp:
// A class that holds information about one of several event types.

namespace hal
{
    namespace proxy
    {
        class events;
    }

    namespace event
    {
        class display : private SDL_DisplayEvent
        {
        public:
            display() = delete;

            hal::display::id_t display_index() const;
            display&           display_index(hal::display::id_t);

            // As of April 2024, there are no events using the "data1" member.
        };

        static_assert(sizeof(display) == sizeof(SDL_DisplayEvent));

        class window : private SDL_WindowEvent
        {
        public:
            window() = delete;

            hal::window::id_t window_id() const;
            window&           window_id(hal::window::id_t id);

            // Valid for: display_changed.
            hal::display::id_t display_index() const;

            // This function automatically sets the event type to display_changed.
            window& display_index(hal::display::id_t id);

            // Valid for: resized, size_changed, moved.
            pixel::point point() const;

            // The provided type must be resized, size_changed, or moved.
            // This function automatically sets the event type.
            window& point(pixel::point pt);
        };

        static_assert(sizeof(window) == sizeof(SDL_WindowEvent));

        class keyboard : private SDL_KeyboardEvent
        {
        public:
            keyboard() = delete;

            hal::window::id_t window_id() const;
            keyboard&         window_id(hal::window::id_t id);

            hal::keyboard::button button() const;
            keyboard&             button(hal::keyboard::button btn);

            hal::keyboard::key key() const;
            keyboard&          key(hal::keyboard::key k);

            bool      repeat() const;
            keyboard& repeat(bool r);
        };

        static_assert(sizeof(keyboard) == sizeof(SDL_KeyboardEvent));

        class mouse_motion : private SDL_MouseMotionEvent
        {
        public:
            mouse_motion() = delete;

            hal::window::id_t window_id() const;
            mouse_motion&     window_id(hal::window::id_t id);

            mouse::state  state() const;
            mouse_motion& state(mouse::state s);

            pixel::point  pos() const;
            mouse_motion& pos(pixel::point p);

            pixel::point  rel() const;
            mouse_motion& rel(pixel::point rel);
        };

        static_assert(sizeof(mouse_motion) == sizeof(SDL_MouseMotionEvent));

        class mouse_button : private SDL_MouseButtonEvent
        {
        public:
            mouse_button() = delete;

            hal::window::id_t window_id() const;
            mouse_button&     window_id(hal::window::id_t id);

            mouse::button button() const;
            mouse_button& button(mouse::button btn);

            std::uint8_t  click_amount() const;
            mouse_button& click_amount(std::uint8_t amnt);

            pixel::point  pos() const;
            mouse_button& pos(pixel::point pt);
        };

        static_assert(sizeof(mouse_button) == sizeof(SDL_MouseButtonEvent));

        class mouse_wheel : private SDL_MouseWheelEvent
        {
        public:
            mouse_wheel() = delete;

            hal::window::id_t window_id() const;
            mouse_wheel&      window_id(hal::window::id_t id);

            pixel::point pos() const;
            mouse_wheel& pos(pixel::point p);

            point<std::int16_t> scroll() const;
            mouse_wheel&        scroll(point<std::int16_t> s);

            point<float> scroll_precise() const;
            mouse_wheel& scroll_precise(point<float> s);

            bool         scroll_flipped() const;
            mouse_wheel& scroll_flipped(bool f);
        };

        static_assert(sizeof(mouse_wheel) == sizeof(SDL_MouseWheelEvent));

        class text_input : private SDL_TextInputEvent
        {
        public:
            hal::window::id_t window_id() const;
            text_input&       window_id(hal::window::id_t id);

            const char* text() const;
            text_input& text(const char* t);
        };

        static_assert(sizeof(text_input) == sizeof(SDL_TextInputEvent));

        // Top-level event types.
        enum class type : std::uint16_t
        {
            quit_requested = SDL_EVENT_QUIT,
            terminating    = SDL_EVENT_TERMINATING,

            low_memory = SDL_EVENT_LOW_MEMORY,

            background_will_enter = SDL_EVENT_WILL_ENTER_BACKGROUND,
            background_entered    = SDL_EVENT_DID_ENTER_BACKGROUND,

            foreground_will_enter = SDL_EVENT_WILL_ENTER_FOREGROUND,
            foreground_entered    = SDL_EVENT_DID_ENTER_FOREGROUND,

            locale_changed = SDL_EVENT_LOCALE_CHANGED,

            system_theme_changed = SDL_EVENT_SYSTEM_THEME_CHANGED,

            display_orientation_changed   = SDL_EVENT_DISPLAY_ORIENTATION,
            display_added                 = SDL_EVENT_DISPLAY_ADDED,
            display_removed               = SDL_EVENT_DISPLAY_REMOVED,
            display_moved                 = SDL_EVENT_DISPLAY_MOVED,
            display_desktop_mode_changed  = SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED,
            display_current_mode_changed  = SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED,
            display_content_scale_changed = SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED,

            window_shown                 = SDL_EVENT_WINDOW_SHOWN,
            window_hidden                = SDL_EVENT_WINDOW_HIDDEN,
            window_exposed               = SDL_EVENT_WINDOW_EXPOSED,
            window_moved                 = SDL_EVENT_WINDOW_MOVED,
            window_resized               = SDL_EVENT_WINDOW_RESIZED,
            window_pixel_size_changed    = SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED,
            window_metal_view_resized    = SDL_EVENT_WINDOW_METAL_VIEW_RESIZED,
            window_minimized             = SDL_EVENT_WINDOW_MINIMIZED,
            window_maximized             = SDL_EVENT_WINDOW_MAXIMIZED,
            window_restored              = SDL_EVENT_WINDOW_RESTORED,
            window_mouse_enter           = SDL_EVENT_WINDOW_MOUSE_ENTER,
            window_mouse_leave           = SDL_EVENT_WINDOW_MOUSE_LEAVE,
            window_focus_gained          = SDL_EVENT_WINDOW_FOCUS_GAINED,
            window_focus_lost            = SDL_EVENT_WINDOW_FOCUS_LOST,
            window_close_requested       = SDL_EVENT_WINDOW_CLOSE_REQUESTED,
            window_hit_test              = SDL_EVENT_WINDOW_HIT_TEST,
            window_icc_profile_changed   = SDL_EVENT_WINDOW_ICCPROF_CHANGED,
            window_display_changed       = SDL_EVENT_WINDOW_DISPLAY_CHANGED,
            window_display_scale_changed = SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED,
            window_safe_area_changed     = SDL_EVENT_WINDOW_SAFE_AREA_CHANGED,
            window_occluded              = SDL_EVENT_WINDOW_OCCLUDED,
            window_fullscreen_entered    = SDL_EVENT_WINDOW_ENTER_FULLSCREEN,
            window_fullscreen_left       = SDL_EVENT_WINDOW_LEAVE_FULLSCREEN,
            window_destroyed             = SDL_EVENT_WINDOW_DESTROYED,
            window_hdr_state_changed     = SDL_EVENT_WINDOW_HDR_STATE_CHANGED,

            key_pressed  = SDL_EVENT_KEY_DOWN,
            key_released = SDL_EVENT_KEY_UP,

            text_composiion             = SDL_EVENT_TEXT_EDITING,
            text_input                  = SDL_EVENT_TEXT_INPUT,
            keymap_changed              = SDL_EVENT_KEYMAP_CHANGED,
            keyboard_added              = SDL_EVENT_KEYBOARD_ADDED,
            keyboard_removed            = SDL_EVENT_KEYBOARD_REMOVED,
            text_composition_candidates = SDL_EVENT_TEXT_EDITING_CANDIDATES,

            mouse_moved       = SDL_EVENT_MOUSE_MOTION,
            mouse_pressed     = SDL_EVENT_MOUSE_BUTTON_DOWN,
            mouse_released    = SDL_EVENT_MOUSE_BUTTON_UP,
            mouse_wheel_moved = SDL_EVENT_MOUSE_WHEEL,
            mouse_added       = SDL_EVENT_MOUSE_ADDED,
            mouse_removed     = SDL_EVENT_MOUSE_REMOVED,

            // [joystick events]
            // [gamepad events]
            // [touch events]

            clipboard_updated = SDL_EVENT_CLIPBOARD_UPDATE,

            drop_file     = SDL_EVENT_DROP_FILE,
            drop_text     = SDL_EVENT_DROP_TEXT,
            drop_begin    = SDL_EVENT_DROP_BEGIN,
            drop_complete = SDL_EVENT_DROP_COMPLETE,
            drop_position = SDL_EVENT_DROP_POSITION,

            audio_device_added          = SDL_EVENT_AUDIO_DEVICE_ADDED,
            audio_device_removed        = SDL_EVENT_AUDIO_DEVICE_REMOVED,
            audio_device_format_changed = SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED,

            // [sensor events]
            // [pen events]
            // [camera events]

            render_targets_reset = SDL_EVENT_RENDER_TARGETS_RESET,
            render_device_reset  = SDL_EVENT_RENDER_DEVICE_RESET,
            render_device_lost   = SDL_EVENT_RENDER_DEVICE_LOST
        };

        class variant
        {
        public:
            variant();

            // Get/set this event's current type.
            type kind() const;
            void kind(type t);

            // Valid for: display
            const event::display& display() const;
            event::display&       display();

            // Valid for: window
            const event::window& window() const;
            event::window&       window();

            // Valid for: key_pressed, key_released
            const event::keyboard& keyboard() const;
            event::keyboard&       keyboard();

            // Valid for: text_input
            const event::text_input& text_input() const;
            event::text_input&       text_input();

            // Valid for: mouse_moved
            const event::mouse_motion& mouse_motion() const;
            event::mouse_motion&       mouse_motion();

            // Valid for: mouse_pressed, mouse_released
            const event::mouse_button& mouse_button() const;
            event::mouse_button&       mouse_button();

            // Valid for: mouse_wheel_moved
            const event::mouse_wheel& mouse_wheel() const;
            event::mouse_wheel&       mouse_wheel();

            // Check whether there are any pending event in the event queue.
            bool pending() const;

            const SDL_Event& get(pass_key<proxy::events>) const;
            SDL_Event&       get(pass_key<proxy::events>);

        private:
            SDL_Event m_event;
        };
    }

    constexpr std::string_view to_string(event::type t)
    {
        using enum event::type;
        using namespace std::string_view_literals;

        switch (t)
        {

        case quit_requested:
            return "Quit requested"sv;

        case terminating:
            return "Terminating"sv;

        case low_memory:
            return "Low memory"sv;

        case background_will_enter:
            return "Will enter background"sv;

        case background_entered:
            return "Entered background"sv;

        case foreground_will_enter:
            return "Will enter foreground"sv;

        case foreground_entered:
            return "Entered foreground"sv;

        case locale_changed:
            return "Locale changed"sv;

        case system_theme_changed:
            return "System theme changed"sv;

        case display_orientation_changed:
            return "Display orientation changed"sv;

        case display_added:
            return "Display added"sv;

        case display_removed:
            return "Display removed"sv;

        case display_moved:
            return "Display moved"sv;

        case display_desktop_mode_changed:
            return "Display desktop mode changed"sv;

        case display_current_mode_changed:
            return "Display current mode changed"sv;

        case display_content_scale_changed:
            return "Display content scale changed"sv;

        case window_shown:
            return "Window shown"sv;

        case window_hidden:
            return "Window hidden"sv;

        case window_exposed:
            return "Window exposed"sv;

        case window_moved:
            return "Window moved"sv;

        case window_resized:
            return "Window resized"sv;

        case window_pixel_size_changed:
            return "Window pixel size changed"sv;

        case window_metal_view_resized:
            return "Window Metal view resized"sv;

        case window_minimized:
            return "Window minimized"sv;

        case window_maximized:
            return "Windowm maximized"sv;

        case window_restored:
            return "Window restored"sv;

        case window_mouse_enter:
            return "Mouse entered window"sv;

        case window_mouse_leave:
            return "Mouse left window"sv;

        case window_focus_gained:
            return "Window gained focus"sv;

        case window_focus_lost:
            return "Window lost focus"sv;

        case window_close_requested:
            return "Window close requested"sv;

        case window_hit_test:
            return "Window hit test"sv;

        case window_icc_profile_changed:
            return "Window ICC profile changed"sv;

        case window_display_changed:
            return "Window display changed"sv;

        case window_display_scale_changed:
            return "Window display scale changed"sv;

        case window_safe_area_changed:
            return "Window safe area changed"sv;

        case window_occluded:
            return "Window occluded"sv;

        case window_fullscreen_entered:
            return "Window entered fullscreen"sv;

        case window_fullscreen_left:
            return "Window left fullscreen"sv;

        case window_destroyed:
            return "Window destroyed"sv;

        case window_hdr_state_changed:
            return "Window HDR state changed"sv;

        case key_pressed:
            return "Key preesed"sv;

        case key_released:
            return "Key released"sv;

        case text_composiion:
            return "Text composition"sv;

        case text_input:
            return "Text input"sv;

        case keymap_changed:
            return "Keymap changed"sv;

        case keyboard_added:
            return "Keyboard added"sv;

        case keyboard_removed:
            return "Keyboard removed"sv;

        case text_composition_candidates:
            return "Text composition candidates"sv;

        case mouse_moved:
            return "Mouse moved"sv;

        case mouse_pressed:
            return "Mouse pressed"sv;

        case mouse_released:
            return "Mouse released"sv;

        case mouse_wheel_moved:
            return "Mouse wheel moved"sv;

        case mouse_added:
            return "Mouse added"sv;

        case mouse_removed:
            return "Mouse removed"sv;

        case clipboard_updated:
            return "Clipboard updated"sv;

        case drop_file:
            return "File drop"sv;

        case drop_text:
            return "Text drop"sv;

        case drop_begin:
            return "Drop begin"sv;

        case drop_complete:
            return "Drop completed"sv;

        case drop_position:
            return "Drop position"sv;

        case audio_device_added:
            return "Audio device added"sv;

        case audio_device_removed:
            return "Audio device removed"sv;

        case audio_device_format_changed:
            return "Audio device format changed"sv;

        case render_targets_reset:
            return "Render targets reset"sv;

        case render_device_reset:
            return "Render device reset"sv;

        case render_device_lost:
            return "Render device lost"sv;
        }
    }

    namespace event
    {
        template <typename T>
            requires requires(const T& x) { hal::to_string(x); }
        std::ostream& operator<<(std::ostream& str, const T& obj)
        {
            return str << hal::to_string(obj);
        }
    }
}
