#pragma once

#include <span>

#include <halcyon/video/texture.hpp>

#include <halcyon/internal/drawer.hpp>
#include <halcyon/internal/scaler.hpp>

#include <halcyon/utility/buffer.hpp>
#include <halcyon/utility/enum_bits.hpp>

// video/renderer.hpp:
// A proxy for creating and rendering textures etc. - more info below.

namespace hal
{
    class surface;

    // Forward declarations for parameters and return types.
    class window;
    class copyer;
    class renderer;
    class texture;

    class static_texture;
    class target_texture;

    enum class flip : u8
    {
        none = SDL_FLIP_NONE,
        x    = SDL_FLIP_HORIZONTAL,
        y    = SDL_FLIP_VERTICAL,
        both = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
    };

    namespace info::sdl
    {
        class renderer;
    }

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
        void clear();

        // Present the back-buffer and clear it.
        void present();

        // Drawing & filling:
        // Each of these functions has an additional overload since
        // creating a color guard is annoying for one-off calls.
        // Keep in mind that the renderer stores its draw color internally,
        // so if you plan to do multiple draw/fill operations, prefer the color guard.

        // Draw a single point (pixel) with the current color.
        void draw(coord::point pt);
        void draw(coord::point pt, color c);

        // Draw a line with the current color.
        void draw(coord::point from, coord::point to);
        void draw(coord::point from, coord::point to, color c);

        // Outline a rectangle with the current color.
        void draw(coord::rect area);
        void draw(coord::rect area, color c);

        // Draw a texture. Returns a builder-like class
        [[nodiscard]] copyer draw(ref<const texture> tx);

        // Fill an area.
        void fill(coord::rect area);
        void fill(coord::rect area, color c);

        // Fill an array of areas.
        void fill(std::span<const coord::rect> areas);
        void fill(std::span<const coord::rect> areas, color c);

        // Fill the entire rendering target.
        void fill();
        void fill(color c);

        // Get/set the rendering target.
        void target(ref<target_texture> tx);
        void reset_target();

        surface read_pixels() const;

        // Read pixels from an area of the rendering target.
        // 'fmt' must be the renderer's primary pixel format.
        // Get it via renderer::info().formats().front().
        surface read_pixels(pixel::rect area) const;

        // Get/set the color used for draw/fill operations.
        hal::color color() const;
        void       color(hal::color clr);

        // Get/set the blend mode used foor draw/fill operations.
        blend_mode blend() const;
        void       blend(blend_mode bm);

        pixel::point size() const;
        void         size(pixel::point sz);
        void         size(scaler scl);

        ref<const window> window() const;
        ref<class window> window();

        info::sdl::renderer info() const;

        // Texture creation functions.
        [[nodiscard]] static_texture make_static_texture(ref<const surface> surf) const&;
        [[nodiscard]] static_texture make_static_texture(ref<const surface> surf) const&& = delete;

        [[nodiscard]] target_texture make_target_texture(pixel::point size, pixel::format) const&;
        [[nodiscard]] target_texture make_target_texture(pixel::point size, pixel::format) const&& = delete;

        [[nodiscard]] streaming_texture make_streaming_texture(pixel::point size, pixel::format fmt) const&;
        [[nodiscard]] streaming_texture make_streaming_texture(pixel::point size, pixel::format fmt) const&& = delete;

    private:
        // Helper for setting the render target.
        void common_target(SDL_Texture* target);
    };

    namespace info
    {
        class renderer;

        namespace sdl
        {
            class renderer : SDL_RendererInfo
            {
            public:
                renderer() = default;

                renderer(ref<const hal::renderer> rnd, pass_key<hal::renderer>);

                std::string_view name() const;

                hal::renderer::flag_bitmask flags() const;

                std::span<const pixel::format> formats() const;

                pixel::point max_texture_size() const;

                SDL_RendererInfo* get();

                friend std::ostream& operator<<(std::ostream& str, const info::sdl::renderer& inf);
            };
        }

        class renderer
        {
        public:
            renderer() = default;

            // Compress native renderer info.
            renderer(const sdl::renderer& src);

            std::string_view name;
            pixel::point     max_texture_size;

            hal::renderer::flag_bitmask flags;
            buffer<pixel::format>       formats;
        };
    }

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
        void operator()();

    private:
        f64 m_angle { 0.0 };

        enum flip m_flip
        {
            flip::none
        };
    };
}
