#pragma once

#include <halcyon/subsystem.hpp>

#include <halcyon/utility/enum_bits.hpp>
#include <halcyon/utility/pass_key.hpp>

#include <halcyon/video/types.hpp>

// keyboard.hpp:
// Keyboard enumerations and access.

namespace hal
{
    namespace proxy
    {
        class events;
    }

    namespace keyboard
    {
        // A representation of a physical, layout- and locale-independent button.
        enum class button : std::uint8_t
        {
            A = SDL_SCANCODE_A,
            B = SDL_SCANCODE_B,
            C = SDL_SCANCODE_C,
            D = SDL_SCANCODE_D,
            E = SDL_SCANCODE_E,
            F = SDL_SCANCODE_F,
            G = SDL_SCANCODE_G,
            H = SDL_SCANCODE_H,
            I = SDL_SCANCODE_I,
            J = SDL_SCANCODE_J,
            K = SDL_SCANCODE_K,
            L = SDL_SCANCODE_L,
            M = SDL_SCANCODE_M,
            N = SDL_SCANCODE_N,
            O = SDL_SCANCODE_O,
            P = SDL_SCANCODE_P,
            Q = SDL_SCANCODE_Q,
            R = SDL_SCANCODE_R,
            S = SDL_SCANCODE_S,
            T = SDL_SCANCODE_T,
            U = SDL_SCANCODE_U,
            V = SDL_SCANCODE_V,
            W = SDL_SCANCODE_W,
            X = SDL_SCANCODE_X,
            Y = SDL_SCANCODE_Y,
            Z = SDL_SCANCODE_Z,

            one   = SDL_SCANCODE_1,
            two   = SDL_SCANCODE_2,
            three = SDL_SCANCODE_3,
            four  = SDL_SCANCODE_4,
            five  = SDL_SCANCODE_5,
            six   = SDL_SCANCODE_6,
            seven = SDL_SCANCODE_7,
            eight = SDL_SCANCODE_8,
            nine  = SDL_SCANCODE_9,

            F1  = SDL_SCANCODE_F1,
            F2  = SDL_SCANCODE_F2,
            F3  = SDL_SCANCODE_F3,
            F4  = SDL_SCANCODE_F4,
            F5  = SDL_SCANCODE_F5,
            F6  = SDL_SCANCODE_F6,
            F7  = SDL_SCANCODE_F7,
            F8  = SDL_SCANCODE_F8,
            F9  = SDL_SCANCODE_F9,
            F10 = SDL_SCANCODE_F10,
            F11 = SDL_SCANCODE_F11,
            F12 = SDL_SCANCODE_F12,

            esc        = SDL_SCANCODE_ESCAPE,
            tab        = SDL_SCANCODE_TAB,
            caps_lock  = SDL_SCANCODE_CAPSLOCK,
            left_shift = SDL_SCANCODE_LSHIFT,
            left_ctrl  = SDL_SCANCODE_LCTRL,

            backspace   = SDL_SCANCODE_BACKSPACE,
            enter       = SDL_SCANCODE_RETURN,
            right_shift = SDL_SCANCODE_RSHIFT,
            right_ctrl  = SDL_SCANCODE_RCTRL,

            left_alt  = SDL_SCANCODE_LALT,
            right_alt = SDL_SCANCODE_RALT,

            up_arrow   = SDL_SCANCODE_UP,
            down_arrow = SDL_SCANCODE_DOWN,

            left_arrow  = SDL_SCANCODE_LEFT,
            right_arrow = SDL_SCANCODE_RIGHT,

            plus  = SDL_SCANCODE_KP_PLUS,
            minus = SDL_SCANCODE_KP_MINUS,
        };

        // A representation of a locale- and layout-dependent key.
        enum class key : std::uint32_t
        {
            A = SDLK_A,
            B = SDLK_B,
            C = SDLK_C,
            D = SDLK_D,
            E = SDLK_E,
            F = SDLK_F,
            G = SDLK_G,
            H = SDLK_H,
            I = SDLK_I,
            J = SDLK_J,
            K = SDLK_K,
            L = SDLK_L,
            M = SDLK_M,
            N = SDLK_N,
            O = SDLK_O,
            P = SDLK_P,
            Q = SDLK_Q,
            R = SDLK_R,
            S = SDLK_S,
            T = SDLK_T,
            U = SDLK_U,
            V = SDLK_V,
            W = SDLK_W,
            X = SDLK_X,
            Y = SDLK_Y,
            Z = SDLK_Z,

            one   = SDLK_1,
            two   = SDLK_2,
            three = SDLK_3,
            four  = SDLK_4,
            five  = SDLK_5,
            six   = SDLK_6,
            seven = SDLK_7,
            eight = SDLK_8,
            nine  = SDLK_9,

            F1  = SDLK_F1,
            F2  = SDLK_F2,
            F3  = SDLK_F3,
            F4  = SDLK_F4,
            F5  = SDLK_F5,
            F6  = SDLK_F6,
            F7  = SDLK_F7,
            F8  = SDLK_F8,
            F9  = SDLK_F9,
            F10 = SDLK_F10,
            F11 = SDLK_F11,
            F12 = SDLK_F12,

            esc        = SDLK_ESCAPE,
            tab        = SDLK_TAB,
            caps_lock  = SDLK_CAPSLOCK,
            left_shift = SDLK_LSHIFT,
            left_ctrl  = SDLK_LCTRL,

            backspace   = SDLK_BACKSPACE,
            enter       = SDLK_RETURN,
            right_shift = SDLK_RSHIFT,
            right_ctrl  = SDLK_RCTRL,

            left_alt  = SDLK_LALT,
            right_alt = SDLK_RALT,

            up_arrow   = SDLK_UP,
            down_arrow = SDLK_DOWN,

            left_arrow  = SDLK_LEFT,
            right_arrow = SDLK_RIGHT,

            plus  = SDLK_PLUS,
            minus = SDLK_MINUS
        };

        enum class mod : std::uint16_t
        {
            none = SDL_KMOD_NONE,

            shift_left  = SDL_KMOD_LSHIFT,
            shift_right = SDL_KMOD_RSHIFT,
            shift_both  = SDL_KMOD_SHIFT,

            ctrl_left  = SDL_KMOD_LCTRL,
            ctrl_right = SDL_KMOD_RCTRL,
            ctrl_both  = SDL_KMOD_CTRL,

            alt_left  = SDL_KMOD_LALT,
            alt_right = SDL_KMOD_RALT,
            alt_both  = SDL_KMOD_ALT,

            gui_left  = SDL_KMOD_LGUI,
            gui_right = SDL_KMOD_RGUI,
            gui_both  = SDL_KMOD_GUI,

            num_lock    = SDL_KMOD_NUM,
            caps_lock   = SDL_KMOD_CAPS,
            mode_lock   = SDL_KMOD_MODE, // What even is this?
            scroll_lock = SDL_KMOD_SCROLL
        };

        key    to_key(button btn);
        button to_button(key k);

        // A reference to the keyboard state. Unlike that mouse state,
        // you can keep this object around, as it always references the current
        // state as long as you keep polling for event in your application loop.
        class state_reference
        {
        public:
            state_reference(pass_key<proxy::events>);

            bool operator[](button b) const;
            bool operator[](key k) const;

        private:
            const bool* m_arr;
        };

        class mod_state : public enum_bitmask<mod, meta::underlying_type<SDL_Keymod>>
        {
        public:
            mod_state(pass_key<proxy::events>);
        };
    }

    c_string to_string(keyboard::button btn);
    c_string to_string(keyboard::key k);

    namespace keyboard
    {
        template <typename T>
            requires requires(const T& x) { hal::to_string(x); }
        std::ostream& operator<<(std::ostream& str, const T& obj)
        {
            return str << hal::to_string(obj);
        }
    }
}
