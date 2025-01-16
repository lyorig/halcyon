#pragma once

#include <halcyon/video/display.hpp>
#include <halcyon/video/renderer.hpp>

#include <halcyon/subsystem.hpp>

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
        using id_t = std::uint8_t;

        consteval static id_t invalid_id()
        {
            return 0;
        }

        consteval static display::id_t invalid_display_index()
        {
            return -1;
        }

        enum class flag : std::uint32_t
        {
            none                = 0,
            fullscreen          = SDL_WINDOW_FULLSCREEN,
            opengl              = SDL_WINDOW_OPENGL,
            occluded            = SDL_WINDOW_OCCLUDED,
            hidden              = SDL_WINDOW_HIDDEN,
            borderless          = SDL_WINDOW_BORDERLESS,
            resizable           = SDL_WINDOW_RESIZABLE,
            minimized           = SDL_WINDOW_MINIMIZED,
            maximized           = SDL_WINDOW_MAXIMIZED,
            mouse_grabbed       = SDL_WINDOW_MOUSE_GRABBED,
            input_focus         = SDL_WINDOW_INPUT_FOCUS,
            mouse_focus         = SDL_WINDOW_MOUSE_FOCUS,
            external            = SDL_WINDOW_EXTERNAL,
            modal               = SDL_WINDOW_MODAL,
            high_pixel_density  = SDL_WINDOW_HIGH_PIXEL_DENSITY,
            mouse_capture       = SDL_WINDOW_MOUSE_CAPTURE,
            mouse_relative_mode = SDL_WINDOW_MOUSE_RELATIVE_MODE,
            always_on_top       = SDL_WINDOW_ALWAYS_ON_TOP,
            utility             = SDL_WINDOW_UTILITY,
            tooltip             = SDL_WINDOW_TOOLTIP,
            popup_menu          = SDL_WINDOW_POPUP_MENU,
            keyboard_grabbed    = SDL_WINDOW_KEYBOARD_GRABBED,
            vulkan              = SDL_WINDOW_VULKAN,
            metal               = SDL_WINDOW_METAL,
            transparent         = SDL_WINDOW_TRANSPARENT,
            not_focusable       = SDL_WINDOW_NOT_FOCUSABLE
        };

        using flag_bitmask = enum_bitmask<flag>;

        window() = default;

        // Create a window with specific flags.
        // If you want fullscreen, a tagged constructor exists for that purpose
        // and using it is recommended.
        window(proxy::video sys, c_string title, pixel::point size, flag_bitmask f = {});

        // Create a window in fullscreen mode.
        window(proxy::video sys, c_string title, HAL_TAG_NAME(fullscreen));

        // Get the index of the display this window is currently on.
        // In case of failure, returns invalid_display_index.
        display::id_t display_index() const;

        pixel::format pixel_format() const;

        // Get this window's ID – used in window events, for example.
        // In case of failure, returns 0.
        id_t id() const;

        flag_bitmask flags() const;

        pixel::point pos() const;
        void         pos(pixel::point ps);

        pixel::point size() const;
        void         size(pixel::point sz);

        pixel::point min_size() const;
        void         min_size(pixel::point sz);

        pixel::point max_size() const;
        void         max_size(pixel::point sz);

        c_string title() const;
        void     title(const char* val);

        // Returns true if the window is fullscreen or fullscreen borderless.
        bool fullscreen() const;

        // Set whether the window is in fullscreen.
        // Returns whether the operation succeeded.
        outcome fullscreen(bool set);

        void always_on_top(bool set);

        ref<const renderer> renderer() const;
        ref<class renderer> renderer();

        // View the surface associated with this window.
        // Only works if a software renderer is used.
        ref<const surface> surface() const;

        friend std::ostream& operator<<(std::ostream& str, hal::ref<hal::window> wnd);
    };
}
