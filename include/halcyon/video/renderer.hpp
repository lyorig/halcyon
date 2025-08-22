#pragma once

#include <halcyon/video/driver.hpp>
#include <halcyon/video/texture.hpp>

#include <halcyon/internal/drawer.hpp>

#include <halcyon/utility/buffer.hpp>
#include <halcyon/utility/enum_bits.hpp>

#include <halcyon/types/c_string.hpp>

#include <halcyon/properties.hpp>

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

    enum class flip : std::uint8_t
    {
        none = SDL_FLIP_NONE,
        x    = SDL_FLIP_HORIZONTAL,
        y    = SDL_FLIP_VERTICAL,
        both = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
    };

    HAL_TAG(connect);

    // A wrapper of SDL_Renderer. Essentially, this is the thing that does the rendering, and
    // is attached to a window. Multiple renderers can exist for a single window, i.e. a hardware-
    // accelerated one, plus a software fallback in case the former isn't available.
    // By default, renderers use hardware acceleration. You can override this via renderer flags.
    class renderer : public detail::resource<SDL_Renderer, ::SDL_DestroyRenderer>
    {
    public:
        class create_properties : public hal::properties
        {
        public:
            create_properties() = default;

            create_properties& name(c_string val);
            create_properties& window(ref<window> val);
            create_properties& surface(ref<surface> val);
            create_properties& vsync(bool val);
        };

        // Everything in here is read-only for now.
        class properties : public hal::properties_ref
        {
        public:
            properties(SDL_PropertiesID id, pass_key<renderer>);

            c_string name() const;

            ref<hal::window>  window() const;
            ref<hal::surface> surface() const;

            bool vsync() const;

            std::int64_t max_texture_size() const;

            // Terminated with hal::pixel::format::unknown.
            const pixel::format* formats() const;
        };

        enum class presentation : std::uint8_t
        {
            disabled      = SDL_LOGICAL_PRESENTATION_DISABLED,
            stretch       = SDL_LOGICAL_PRESENTATION_STRETCH,
            letterbox     = SDL_LOGICAL_PRESENTATION_LETTERBOX,
            overscan      = SDL_LOGICAL_PRESENTATION_OVERSCAN,
            integer_scale = SDL_LOGICAL_PRESENTATION_INTEGER_SCALE
        };

        renderer() = default;

        renderer(lref<const hal::window> wnd);
        renderer(const create_properties& props);

        // Clear (fill) the render target with the current draw color.
        bool clear();

        // Present the back-buffer.
        // This function doesn't clear afterwards; use `renderer::present_and_clear()` for that.
        bool present();

        // Present and clear the back-buffer.
        // This function returns `false` if either operation fails.
        bool present_and_clear();

        bool flush();

        // Drawing & filling:
        // Each of these functions has an additional overload since
        // creating a color guard is annoying for one-off calls.
        // Keep in mind that the renderer stores its draw color internally,
        // so if you plan to do multiple draw/fill operations, prefer the color guard.

        // Draw a single point (pixel) with the current color.
        bool draw(coord::point pt);
        bool draw(coord::point pt, color c);

        bool draw(std::span<const coord::point> pts);
        bool draw(std::span<const coord::point> pts, color c);

        // Draw a line with the current color.
        bool draw(coord::point from, coord::point to);
        bool draw(coord::point from, coord::point to, color c);

        // Draw a connected series of points with the current color.
        bool draw(std::span<const coord::point> pt, HAL_TAG_NAME(connect));
        bool draw(std::span<const coord::point> pt, color c, HAL_TAG_NAME(connect));

        // Outline a rectangle with the current color.
        bool draw(coord::rect area);
        bool draw(coord::rect area, color c);

        // Draw multiple rectangles with the current color.
        bool draw(std::span<const coord::rect> pt);
        bool draw(std::span<const coord::rect> pt, color c);

        // Draw a texture. Returns a builder-like class.
        [[nodiscard]] copyer draw(ref<const texture> tx);

        // Fill an area.
        bool fill(coord::rect area);
        bool fill(coord::rect area, color c);

        // Fill an array of areas.
        bool fill(std::span<const coord::rect> areas);
        bool fill(std::span<const coord::rect> areas, color c);

        // Fill the entire rendering target.
        bool fill();
        bool fill(color c);

        // Get/set the rendering target.
        bool target(ref<target_texture> tx);
        bool reset_target();

        surface read_pixels() const;

        // Read pixels from an area of the rendering target.
        // 'fmt' must be the renderer's primary pixel format.
        surface read_pixels(pixel::rect area) const;

        // Get/set the color used for draw/fill operations.
        result<hal::color> color() const;
        bool               color(hal::color clr);

        // Get/set the blend mode used foor draw/fill operations.
        result<blend_mode> blend() const;
        bool               blend(blend_mode bm);

        result<pixel::point> size() const;
        bool                 size(pixel::point sz, presentation p);

        ref<const hal::window> window() const;
        ref<hal::window>       window();

        // Get/set vertical sync.
        // Also see `SDL_RENDER_VSYNC_{DISABLED, ADAPTIVE}`.
        result<int> vsync() const;
        bool        vsync(int v);

        const char* name() const;

        properties props() const;

    private:
        // Helper for setting the render target.
        bool internal_target(SDL_Texture* target);
    };

    // A builder-pattern class that facilitates rendering textures.
    // Errors (most commonly an invalid texture) are communicated via the bool
    // returned from operator().
    class copyer : public detail::drawer<const texture, renderer, coord_t, copyer>
    {
    public:
        using drawer::drawer;

        // Outlines the current destination with the renderer's draw color.
        // Can be called at any time, but you probably want to call this
        // after properly setting the destination rectangle.
        // Warning: This outlines the "inner" border, meaning if the texture
        // is opaque, the outline won't be visible.
        [[nodiscard]] copyer& outline();

        // Outlines the current destination with a certain color.
        // Can be called at any time, but you probably want to call this
        // after properly setting the destination rectangle.
        [[nodiscard]] copyer& outline(color c);

        // Finish the operation.
        bool render();
        bool rotated(double angle, flip f);
        bool affine(coord::point right, coord::point down);
        bool tiled(float scale);
        bool nine_grid(float width_left, float width_right, float height_top, float height_bottom, float scale);
    };
}
