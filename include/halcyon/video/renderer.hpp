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
    class renderer : public detail::raii_object<SDL_Renderer, &::SDL_DestroyRenderer>
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

        using flag_bitmask = enum_bitmask<flag, u32>;

        renderer() = default;

        renderer(clref<window> wnd, flag_bitmask f = {});

        // Clear (fill) the render target with the current draw color.
        void clear();

        // Present the back-buffer and clear it.
        void present();

        // Draw a single point (pixel) with the current color.
        void draw(coord::point pt);

        // Draw a line with the current color.
        void draw(coord::point from, coord::point to);

        // Outline a rectangle with the current color.
        void draw(coord::rect area);

        void fill(coord::rect area);
        void fill(std::span<const coord::rect> areas);
        void fill();

        // Get/set the rendering target.
        void target(ref<target_texture> tx);
        void reset_target();

        // Read pixels from the rendering target.
        void read_pixels(int pitch, std::byte* pixels, pixel::format fmt = static_cast<pixel::format>(0));

        // Read pixels from an area of the rendering target.
        void read_pixels(pixel::rect area, int pitch, std::byte* pixels, pixel::format fmt = static_cast<pixel::format>(0));

        // Get/set the color used for draw/fill operations.
        hal::color color() const;
        void       color(hal::color clr);

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

        [[nodiscard]] target_texture make_target_texture(pixel::point size, pixel::format fmt = texture::default_pixel_format) const&;
        [[nodiscard]] target_texture make_target_texture(pixel::point size, pixel::format fmt = texture::default_pixel_format) const&& = delete;

        [[nodiscard]] streaming_texture make_streaming_texture(pixel::point size, pixel::format fmt = texture::default_pixel_format) const&;
        [[nodiscard]] streaming_texture make_streaming_texture(pixel::point size, pixel::format fmt = texture::default_pixel_format) const&& = delete;

        // Render a texture via a builder.
        [[nodiscard]] copyer render(ref<const texture> tex);

    private:
        // Helper for setting the render target.
        void internal_target(SDL_Texture* target);
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
