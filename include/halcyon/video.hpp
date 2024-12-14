#pragma once

#include <halcyon/events.hpp>

#include <halcyon/video/display.hpp>
#include <halcyon/video/driver.hpp>
#include <halcyon/video/message_box.hpp>
#include <halcyon/video/renderer.hpp>
#include <halcyon/video/texture.hpp>
#include <halcyon/video/window.hpp>

#include <halcyon/types/string.hpp>

#include <halcyon/misc.hpp>

// video.hpp:
// Master include file for Halcyon Video.

namespace hal
{
    namespace proxy
    {
        class video
        {
        public:
            HAL_NO_SIZE proxy::events events;

            string  clipboard() const;
            outcome clipboard(const char* text);
            bool    clipboard_has_text() const;

            result<display> display_info_native(display::id_t index) const;
            result<display> display_info_current(display::id_t index) const;

        protected:
            video();
        };
    }
}
