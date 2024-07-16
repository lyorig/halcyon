#pragma once

#include <halcyon/video/display.hpp>
#include <halcyon/video/renderer.hpp>

#include <halcyon/internal/scaler.hpp>

#include <halcyon/internal/subsystem.hpp>
#include <halcyon/surface.hpp>

// video/window.hpp:
// A window. Can't do much by itself.

namespace hal
{
    class window;

    template <>
    class view<const window> : public detail::view_base<SDL_Window>
    {
    protected:
        using view_base::view_base;

    public:
        view(view<const renderer>, pass_key<view<const renderer>>);

        pixel::point pos() const;

        pixel::point size() const;

        // Get the index of the display this window is currently on.
        display::id_t display_index() const;

        pixel::format pixel_format() const;

        std::string_view title() const;

        u8 id() const;

        // Returns true if the window is fullscreen or fullscreen borderless.
        bool fullscreen() const;

        // View the surface associated with this window.
        view<const surface> surface() const;
    };

    template <>
    class view<window> : public view<const window>
    {
        using super = view<const window>;

    protected:
        using super::super;

    public:
        view(view<renderer> rnd, pass_key<view<renderer>>);

        [[nodiscard]] renderer make_renderer(std::initializer_list<renderer::flags> flags = {}) &;

        using super::pos;
        void pos(pixel::point ps);

        using super::size;
        void size(pixel::point sz);
        void size(scaler scl);

        using super::title;
        void title(std::string_view val);

        using super::fullscreen;
        void fullscreen(bool set);
    };

    HAL_TAG(fullscreen);

    // A window. Not much more to say.
    class window : public detail::raii_object<window, ::SDL_DestroyWindow>
    {
    public:
        using id_t = u8;

        using authority_t = hal::proxy::video;

        enum class flags : u16
        {
            none                  = 0,
            fullscreen            = SDL_WINDOW_FULLSCREEN,
            fullscreen_borderless = SDL_WINDOW_FULLSCREEN_DESKTOP,
            hidden                = SDL_WINDOW_HIDDEN,
            resizable             = SDL_WINDOW_RESIZABLE,
            minimized             = SDL_WINDOW_MINIMIZED,
            maximized             = SDL_WINDOW_MAXIMIZED
        };

        window() = default;

        window(proxy::video& sys, std::string_view title, pixel::point size, std::initializer_list<flags> flags = {});

        // Create a window in fullscreen mode.
        // Warning: This has some issues on macOS due to its DPI scaling stuff.
        window(proxy::video& sys, std::string_view title, HAL_TAG_NAME(fullscreen));
    };
}
