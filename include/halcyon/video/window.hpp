#pragma once

#include <halcyon/video/display.hpp>
#include <halcyon/video/renderer.hpp>

#include <halcyon/internal/scaler.hpp>
#include <halcyon/internal/system.hpp>

#include <halcyon/surface.hpp>

// video/window.hpp:
// A window. Can't do much by itself.

namespace hal
{
    HAL_TAG(fullscreen);

    namespace proxy
    {
        class video;
    }

    // A window. Not much more to say.
    class window : public detail::resource<SDL_Window, ::SDL_DestroyWindow>
    {
    public:
        using id_t = u8;

        enum class flag : u16
        {
            none                  = 0,
            fullscreen            = SDL_WINDOW_FULLSCREEN,
            fullscreen_borderless = SDL_WINDOW_FULLSCREEN_DESKTOP,
            hidden                = SDL_WINDOW_HIDDEN,
            resizable             = SDL_WINDOW_RESIZABLE,
            minimized             = SDL_WINDOW_MINIMIZED,
            maximized             = SDL_WINDOW_MAXIMIZED
        };

        using flag_bitmask = enum_bitmask<flag>;

        window() = default;

        // Create a window with specific flags.
        // If you want fullscreen, a tagged constructor exists for that purpose
        // and using it is recommended.
        window(proxy::video sys, std::string_view title, pixel::point size, flag_bitmask f = {});

        // Create a window in fullscreen mode.
        window(proxy::video sys, std::string_view title, HAL_TAG_NAME(fullscreen));

        // Get the index of the display this window is currently on.
        display::id_t display_index() const;

        pixel::format pixel_format() const;

        u8 id() const;

        flag_bitmask flags() const;

        pixel::point pos() const;
        void         pos(pixel::point ps);

        pixel::point size() const;
        void         size(pixel::point sz);
        void         size(scaler scl);

        std::string_view title() const;
        void             title(std::string_view val);

        // Returns true if the window is fullscreen or fullscreen borderless.
        bool fullscreen() const;
        void fullscreen(bool set);

        ref<const renderer> renderer() const;
        ref<class renderer> renderer();

        // View the surface associated with this window.
        // Only works if a software renderer is used.
        ref<const surface> surface() const;

        [[nodiscard]] class renderer make_renderer(renderer::flag_bitmask flags = {});
    };
}
