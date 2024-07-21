#pragma once

#include <span>

#include <SDL_surface.h>

#include <halcyon/internal/drawer.hpp>
#include <halcyon/internal/raii_object.hpp>
#include <halcyon/internal/rwops.hpp>
#include <halcyon/internal/scaler.hpp>

#include <halcyon/types/color.hpp>

// surface.hpp:
// A software bitmap, from which textures are created.
// Offers fast pixel retrieval and manipulation, which textures do not,
// but surface-on-surface blitting is slower due to not being GPU accelerated.

struct SDL_Window;

namespace hal
{
    // Forward definitions for helper classes.
    class const_pixel_reference;
    class pixel_reference;
    class blitter;
    class window;

    namespace image
    {
        class context;
    }

    namespace builder
    {
        class font_text;
        class font_glyph;
    }

    class surface : public detail::raii_object<SDL_Surface, ::SDL_FreeSurface>
    {
    public:
        static constexpr pixel::format default_pixel_format { pixel::format::rgba32 };

        surface() = default;

        // Create a sized surface with an optional pixel format.
        surface(pixel::point sz, pixel::format fmt = default_pixel_format);

        // Load a BMP image. This works natively without having to initialize anything.
        surface(accessor src);

        // [private] Surfaces are converted with view::surface::convert().
        surface(SDL_Surface*, pass_key<surface>);

        // [private] Images are loaded with image::context::load().
        surface(SDL_Surface* ptr, pass_key<image::context>);

        // [private] Text is rendered with ttf::font::render().
        surface(SDL_Surface* ptr, pass_key<builder::font_text>);

        // [private] Glyphs are rendered with ttf::font::render();
        surface(SDL_Surface* ptr, pass_key<builder::font_glyph>);

        // Fill the entire surface with a color.
        void fill(color clr);

        // Fill a rectangle with a color.
        void fill(pixel::rect area, color clr);

        // Fill an array of rectangles with a color.
        void fill(std::span<const pixel::rect> areas, color clr);

        void lock();
        void unlock();

        // Save this surface in a BMP format.
        // More formats are made available by the image context.
        void save(outputter dst) const;

        // Create a blitter.
        // Not all pixel formats can be blitted,
        [[nodiscard]] blitter blit(surface& dst) const;

        // Convert this surface into a blittable format.
        // Use with text.
        [[nodiscard]] surface convert(pixel::format fmt) const;

        // Get surface dimensions.
        pixel::point size() const;

        // Get this surface's pixel format.
        pixel::format pixel_format() const;

        // Get a resized copy of the surface. Useful for saving
        // memory after converting to a texture.
        surface resize(pixel::point sz) const;
        surface resize(scaler scl) const;

        // Whether the surface must be locked before reading/writing pixels.
        bool must_lock() const;

        // Get pixel at position.
        // It is recommended do manipulate pixels at the surface stage,
        // as textures are very slow to retrieve pixel information.
        const_pixel_reference operator[](pixel::point pos) const;
        pixel_reference       operator[](pixel::point pt);

        // Get/set this surface's blend mode.
        blend_mode blend() const;
        void       blend(blend_mode bm);

        // Get/set this surface's color modifiers.
        color color_mod() const;
        void  color_mod(color col);

        // Get/set this surface's alpha modifier.
        color::value_t alpha_mod() const;
        void           alpha_mod(color::value_t val);
    };

    class const_pixel_reference
    {
    public:
        const_pixel_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos, pass_key<surface>);

        color color() const;

    protected:
        const_pixel_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos);
        // Retrieve the color in a mapped format.
        Uint32 get_mapped() const;

        std::byte*             m_ptr; // A pointer to the current color.
        const SDL_PixelFormat* m_fmt;
    };

    // A reference to a pixel in a surface for easy access and modification.
    class pixel_reference : public const_pixel_reference
    {
    public:
        pixel_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos, pass_key<surface>);

        using const_pixel_reference::color;
        void color(hal::color c);

    private:
        // Set the color to a mapped value.
        void set_mapped(Uint32 mv);
    };

    HAL_TAG(keep_dst);

    // A builder pattern drawing proxy for surfaces.
    class blitter : public detail::drawer<const surface, pixel_t, surface, blitter>
    {
    public:
        using drawer::drawer;

        // Finish the operation.
        // SDL's blitting function overwrites the destination rectangle.
        // If you with to reuse this object, use the "keep_dst" overload.
        void operator()();

        // Finish the operation.
        // SDL's blitting function overwrites the destination rectangle.
        // This overload uses a copy to ensure it remains unchanged.
        void operator()(HAL_TAG_NAME(keep_dst)) const;
    };
}
