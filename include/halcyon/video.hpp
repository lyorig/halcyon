#pragma once

#include <halcyon/events.hpp>

#include <halcyon/video/display.hpp>
#include <halcyon/video/driver.hpp>
#include <halcyon/video/message_box.hpp>
#include <halcyon/video/renderer.hpp>
#include <halcyon/video/texture.hpp>
#include <halcyon/video/window.hpp>

#include <halcyon/types/string.hpp>
// video.hpp:
// Master include file for Halcyon Video.

namespace hal
{
    namespace proxy
    {
        class video
        {
        public:
            constexpr static system system { system::video };

            HAL_NO_SIZE proxy::events events;

            string  clipboard() const;
            outcome clipboard(const char* text);
            bool    clipboard_has_text() const;

            outcome display_info_native(display& d, display::id_t index) const;
            outcome display_info_current(display& d, display::id_t index) const;

        protected:
            video();
        };
    }
}
