#pragma once

#include <halcyon/context.hpp>

#include <halcyon/image.hpp>
#include <halcyon/surface.hpp>
#include <halcyon/ttf.hpp>

#include <halcyon/events.hpp>

#include <halcyon/video/display.hpp>
#include <halcyon/video/driver.hpp>
#include <halcyon/video/message_box.hpp>
#include <halcyon/video/renderer.hpp>
#include <halcyon/video/window.hpp>

#include <halcyon/internal/string.hpp>
#include <halcyon/internal/subsystem.hpp>

namespace hal
{
    namespace proxy
    {
        class clipboard
        {
        public:
            clipboard(pass_key<video>);

            // Get current clipboard text.
            // This has to be freed, so a custom wrapper is returned.
            string text() const;

            void text(std::string_view what);

            bool has_text() const;
        };

        class display
        {
        public:
            display(pass_key<video>);

            // Get the amount of video displays.
            hal::display::id_t size() const;

            // Get the name of a display.
            std::string_view name(hal::display::id_t id) const;

            info::sdl::display operator[](hal::display::id_t idx) const;
        };
    }

    namespace detail
    {
        template <>
        class subsystem<system::video>
        {
        public:
            subsystem(pass_key<hal::system::video>);

            [[nodiscard]] window make_window(std::string_view title, pixel::point size, window::flag_bitmask flags = {}) const&;
            [[nodiscard]] window make_window(std::string_view title, pixel::point size, window::flag_bitmask flags = {}) const&& = delete;

            [[nodiscard]] window make_window(std::string_view title, HAL_TAG_NAME(fullscreen)) const&;
            [[nodiscard]] window make_window(std::string_view title, HAL_TAG_NAME(fullscreen)) const&& = delete;

            HAL_NO_SIZE proxy::events events;

            HAL_NO_SIZE proxy::clipboard clipboard;
            HAL_NO_SIZE proxy::display displays;

        private:
            subsystem();
        };
    }
}
