#pragma once

#include <halcyon/misc.hpp>

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
#include <halcyon/internal/system.hpp>

// video.hpp:
// Master include file for Halcyon Video.

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

            // Set clipboard text.
            // Returns whether the operation succeeded.
            bool text(const char* what);

            // Check whether the clipboard contains text data.
            bool has_text() const;
        };

        class display
        {
        public:
            display(pass_key<video>);

            // Get the amount of video displays.
            // Returns zero if the query failed.
            hal::display::id_t size() const;

            // Get the name of a display.
            // Returns nullptr in case of failure.
            const char* name(hal::display::id_t id) const;

            // Get info about a display.
            info::sdl::display operator[](hal::display::id_t idx) const;
        };
    }

    namespace proxy
    {
        class video : public system::base<system::type::video>
        {
        public:
            [[nodiscard]] window make_window(const char* title, pixel::point size, window::flag_bitmask flags = {});
            [[nodiscard]] window make_window(const char* title, HAL_TAG_NAME(fullscreen));

            HAL_NO_SIZE proxy::events events;

            HAL_NO_SIZE proxy::clipboard clipboard;
            HAL_NO_SIZE proxy::display displays;

        protected:
            video();
        };
    }

    namespace system
    {
        using video = guard<proxy::video>;
    }
}
