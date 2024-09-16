#pragma once

#include <halcyon/video/window.hpp>

#include "SDL_messagebox.h"

#include <string_view>
#include <vector>

namespace hal
{
    namespace message_box
    {
        using button_t = u8;

        enum class type : u8
        {
            info    = SDL_MESSAGEBOX_INFORMATION,
            warning = SDL_MESSAGEBOX_WARNING,
            error   = SDL_MESSAGEBOX_ERROR
        };

        enum class default_key : u8
        {
            none   = 0,
            enter  = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
            escape = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT
        };

        // Show a simple message box. For more customization options, create a builder.
        void show(type tp, std::string_view title, std::string_view body);

        class builder
        {
            using this_ref = builder&;

        public:
            builder();

            // Set the title text of the message box.
            // Can be called at any time.
            [[nodiscard]] this_ref title(std::string_view text);

            // Set the body text of the message box.
            // Can be called at any time.
            [[nodiscard]] this_ref body(std::string_view text);

            // Set the type of the message box.
            // Can be called at any time.
            [[nodiscard]] this_ref type(type tp);

            // Set the amount of buttons along with their contents.
            // Call before setting enter/escape defaults.
            [[nodiscard]] this_ref buttons(std::initializer_list<std::string_view> names);

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
            button_t operator()();

        private:
            std::vector<SDL_MessageBoxButtonData> m_btn;

            SDL_MessageBoxData        m_data;
            SDL_MessageBoxColorScheme m_col;
        };
    };
}