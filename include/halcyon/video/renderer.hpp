#pragma once

#include <halcyon/video/driver.hpp>
#include <halcyon/video/texture.hpp>

#include <halcyon/internal/drawer.hpp>
#include <halcyon/internal/scaler.hpp>

#include <halcyon/utility/buffer.hpp>
#include <halcyon/utility/enum_bits.hpp>

#include <halcyon/types/c_string.hpp>

// video/renderer.hpp:
// A proxy for creating and rendering textures etc. - more info below.

namespace hal
{
    // Forward declarations for parameters and return types.
    class surface;
    class window;
    class copyer;
    class renderer;
    class texture;

    class static_texture;
    class target_texture;

    class renderer_info;

    enum class flip : u8
    {
        none = SDL_FLIP_NONE,
        x    = SDL_FLIP_HORIZONTAL,
        y    = SDL_FLIP_VERTICAL,
        both = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
    };

    // A wrapper of SDL_Renderer. Essentially, this is the thing that does the rendering, and
    // is attached to a window. Multiple renderers can exist for a single window, i.e. a hardware-
    // accelerated one, plus a software fallback in case the former isn't available.
    // By default, renderers use hardware acceleration. You can override this via renderer flags.
    class renderer : public detail::resource<SDL_Renderer, &::SDL_DestroyRenderer>
    {
    public:
        enum class flag : u8
        {
            none           = 0,
            software       = SDL_RENDERER_SOFTWARE,     // Require a software renderer.
            accelerated    = SDL_RENDERER_ACCELERATED,  // Require an accelerated (hardware) renderer.
            vsync          = SDL_RENDERER_PRESENTVSYNC, // Require + enable Vsync.
            target_texture = SDL_RENDERER_TARGETTEXTURE // Require support for rendering to a target texture.
        };

        using flag_bitmask = enum_bitmask<flag>;

        renderer() = default;

        renderer(lref<const window> wnd, flag_bitmask f = {});

        // Clear (fill) the render target with the current draw color.
        outcome clear();

        // Present the back-buffer and clear it.
        outcome present();

        // Drawing & filling:
        // Each of these functions has an additional overload since
        // creating a color guard is annoying for one-off calls.
        // Keep in mind that the renderer stores its draw color internally,
        // so if you plan to do multiple draw/fill operations, prefer the color guard.

        // Draw a single point (pixel) with the current color.
        outcome draw(coord::point pt);
        outcome draw(coord::point pt, color c);

        // Draw a line with the current color.
        outcome draw(coord::point from, coord::point to);
        outcome draw(coord::point from, coord::point to, color c);

        // Outline a rectangle with the current color.
        outcome draw(coord::rect area);
        outcome draw(coord::rect area, color c);

        // Draw a texture. Returns a builder-like class
        [[nodiscard]] copyer draw(ref<const texture> tx);

        // Fill an area.
        outcome fill(coord::rect area);
        outcome fill(coord::rect area, color c);

        // Fill an array of areas.
        outcome fill(std::span<const coord::rect> areas);
        outcome fill(std::span<const coord::rect> areas, color c);

        // Fill the entire rendering target.
        outcome fill();
        outcome fill(color c);

        // Get/set the rendering target.
        outcome target(ref<target_texture> tx);
        outcome reset_target();

        surface read_pixels() const;

        // Read pixels from an area of the rendering target.
        // 'fmt' must be the renderer's primary pixel format.
        // Get it via renderer::info().formats().front().
        surface read_pixels(pixel::rect area) const;

        // Get/set the color used for draw/fill operations.
        result<hal::color> color() const;
        outcome            color(hal::color clr);

        // Get/set the blend mode used foor draw/fill operations.
        result<blend_mode> blend() const;
        outcome            blend(blend_mode bm);

        result<pixel::point> size() const;
        outcome              size(pixel::point sz);
        outcome              size(scaler scl);

        ref<const window> window() const;
        ref<class window> window();

        result<renderer_info> info() const;

    private:
        // Helper for setting the render target.
        outcome internal_target(SDL_Texture* target);
    };

    class renderer_info : private SDL_RendererInfo
    {
    public:
        renderer_info() = default;

        c_string name() const;

        renderer::flag_bitmask flags() const;

        std::span<const pixel::format> formats() const;

        pixel::point max_texture_size() const;

        friend result<renderer_info> renderer::info() const;
        friend outcome               driver::info(renderer_info& info, driver::index_t index);

        friend std::ostream& operator<<(std::ostream& str, const renderer_info& inf);
    };

    class copyer : public detail::drawer<const texture, coord_t, renderer, copyer>
    {
    public:
        using drawer::drawer;

        // Set the texture's rotation.
        // Can be called at any time.
        [[nodiscard]] copyer& rotate(f64 angle);

        // Set the texture's flip.
        // Can be called at any time.
        [[nodiscard]] copyer& flip(enum flip f);

        // Outlines the current destination with the renderer's draw color.
        // Can be called at any time, but you probably want to call this
        // after properly setting the destination rectangle.
        [[nodiscard]] copyer& outline();

        // Outlines the current destination with a certain color.
        // Can be called at any time, but you probably want to call this
        // after properly setting the destination rectangle.
        [[nodiscard]] copyer& outline(color c);

        // Finish the operation.
        outcome operator()();

    private:
        f64 m_angle { 0.0 };

        enum flip m_flip
        {
            flip::none
        };
    };
}
