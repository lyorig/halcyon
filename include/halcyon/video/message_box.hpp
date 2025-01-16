#pragma once

#include <halcyon/video/window.hpp>

#include <halcyon/utility/buffer.hpp>

#include "SDL3/SDL_messagebox.h"

namespace hal
{
    namespace message_box
    {
        using button_t = std::uint8_t;

        enum class type : std::uint8_t
        {
            info    = SDL_MESSAGEBOX_INFORMATION,
            warning = SDL_MESSAGEBOX_WARNING,
            error   = SDL_MESSAGEBOX_ERROR
        };

        enum class default_key : std::uint8_t
        {
            none   = 0,
            enter  = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
            escape = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT
        };

        // Show a simple message box. For more customization options, create a builder.
        outcome show(type tp, const char* title, const char* body);

        class builder
        {
        private:
            using this_ref = builder&;

        public:
            consteval static button_t invalid_button()
            {
                return -1;
            }

            builder();

            // Set the title text of the message box.
            // Can be called at any time.
            [[nodiscard]] this_ref title(const char* text);

            // Set the body text of the message box.
            // Can be called at any time.
            [[nodiscard]] this_ref body(const char* text);

            // Set the type of the message box.
            // Can be called at any time.
            [[nodiscard]] this_ref type(type tp);

            // Set the amount of buttons along with their contents.
            // Call before setting enter/escape defaults.
            [[nodiscard]] this_ref buttons(std::initializer_list<const char*> names);

            // Set the message box colors. Only works on certain platforms (e.g. X11, Android).
            // Can be called at any time.
            [[nodiscard]] this_ref colors(color bg, color text, color btn_border, color btn_bg, color btn_select);

            // Set which button is chosen by default upon hitting enter.
            // Call after setting up buttons.
            [[nodiscard]] this_ref enter(button_t idx);

            // Set which button is chosen by default upon hitting escape.
            // Call after setting up buttons.
            [[nodiscard]] this_ref escape(button_t idx);

            // Set this message box's parent window.
            // Can be called at any time.
            [[nodiscard]] this_ref parent(window& wnd);

            // Create the message box.
            // Returns the ID of the button that was pressed.
            // On failure, returns invalid_button.
            button_t operator()();

        private:
            buffer<SDL_MessageBoxButtonData> m_btn;

            SDL_MessageBoxData        m_data;
            SDL_MessageBoxColorScheme m_col;
        };
    };
}
