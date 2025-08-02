#pragma once

#include <halcyon/video/display.hpp>
#include <halcyon/video/renderer.hpp>

#include <halcyon/subsystem.hpp>

#include <halcyon/surface.hpp>

// video/window.hpp:
// A window. Can't do much by itself.

namespace hal
{
    namespace proxy
    {
        class video;
    }

    // A window. Not much more to say.
    class window : public detail::resource<SDL_Window, ::SDL_DestroyWindow>
    {
    public:
        using id_t = std::uint8_t;

        class create_properties : public properties
        {
        public:
            create_properties& always_on_top(bool val);
            create_properties& external_graphics_context(bool val);

            create_properties& focusable(bool val);
            create_properties& resizable(bool val);

            create_properties& hidden(bool val);
            create_properties& high_pixel_density(bool val);

            create_properties& maximized(bool val);
            create_properties& minimized(bool val);
            create_properties& fullscreen(bool val);
            create_properties& borderless(bool val);
            create_properties& transparent(bool val);

            create_properties& menu(bool val);
            create_properties& modal(bool val);
            create_properties& tooltip(bool val);
            create_properties& utility(bool val);
            create_properties& constrain_popup(bool val);

            create_properties& mouse_grabbed(bool val);

            create_properties& metal(bool val);
            create_properties& opengl(bool val);
            create_properties& vulkan(bool val);

            create_properties& parent(ref<window> val);

            create_properties& title(c_string val);

            create_properties& size(pixel::point val);
            create_properties& pos(pixel::point val);
        };

        class properties : public properties_ref
        {
        public:
            properties(id_t id, pass_key<window>);

            // The shape associated with a shaped window.
            ref<hal::surface> shape() const;

            bool  hdr() const;
            float hdr_headroom() const;

            float sdr_white_level() const;
        };

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
        // If `fullscreen` is included in the flags, the size is ignored.
        window(proxy::video sys, c_string title, pixel::point size, flag_bitmask f = {});

        // Create a window with a set of properties.
        window(proxy::video sys, const create_properties& props);

        // Get the index of the display this window is currently on.
        // In case of failure, returns 0.
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
        bool fullscreen(bool set);

        void always_on_top(bool set);

        ref<const hal::renderer> renderer() const;
        ref<class hal::renderer> renderer();

        // View the surface associated with this window.
        // Only works if a software renderer is used.
        ref<const hal::surface> surface() const;

        properties props() const;

        friend std::ostream& operator<<(std::ostream& str, hal::ref<hal::window> wnd);
    };
}
