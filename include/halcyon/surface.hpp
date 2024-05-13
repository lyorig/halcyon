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

namespace hal
{
    // Forward definitions for helper classes.
    class pixel_reference;
    class blitter;

    namespace image
    {
        class context;
    }

    namespace builder
    {
        class font_text;
        class font_glyph;
    }

    HAL_TAG(keep_dst);

    extern template class detail::raii_object<SDL_Surface, ::SDL_FreeSurface>;

    class surface : public detail::raii_object<SDL_Surface, ::SDL_FreeSurface>
    {
    public:
        // Create an invalid surface.
        surface() = default;

        // Create a sized surface.
        surface(pixel_point sz);

        // Load a BMP image. This works natively without having to initialize anything.
        surface(accessor src);

        // [private] Images are loaded with image::context::load().
        surface(SDL_Surface* ptr, pass_key<image::context>);

        // [private] Text is rendered with ttf::font::render().
        surface(SDL_Surface* ptr, pass_key<builder::font_text>);

        // [private] Glyphs are rendered with ttf::font::render();
        surface(SDL_Surface* ptr, pass_key<builder::font_glyph>);

        // Fill the entire surface with a color.
        void fill(color clr);

        // Fill a rectangle with a color.
        void fill_rect(const sdl::pixel_rect& area, color clr);

        // Fill an array of rectangles with a color.
        void fill_rects(const std::span<const sdl::pixel_rect>& areas, color clr);

        // Get a resized copy of the surface. Useful for saving
        // memory after converting to a texture.
        surface resize(pixel_point sz);
        surface resize(scaler scl);

        // Save this surface in a BMP format.
        // More formats are made available by the image context.
        void save(outputter dst) const;

        // Create a blitter.
        [[nodiscard]] blitter blit(surface& dst) const;

        // Convert this surface into a blittable format.
        // Use with text.
        [[nodiscard]] surface convert() const;

        // Get surface dimensions.
        pixel_point size() const;

        // Get/set this surface's blend mode.
        blend_mode blend() const;
        void       blend(blend_mode bm);

        // Get pixel at position.
        // This functionality is exclusive to surfaces, as textures
        // are extremely slow to retrieve pixel information.
        pixel_reference operator[](const pixel_point& pos) const;

    private:
        // [private] Construct a new surface from an existing one
        // and convert it to a specified format.
        surface(const surface& cvt, SDL_PixelFormatEnum fmt);

        // Convert a color to a mapped value using this surface's pixel format.
        std::uint32_t mapped(color c) const;
    };

    // A reference to a pixel in a surface for easy access and modification.
    class pixel_reference
    {
    public:
        pixel_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel_point pos, pass_key<surface>);

        color color() const;
        void  color(hal::color c);

    private:
        // Retrieve the color in a mapped format.
        std::uint32_t get_mapped() const;

        // Set the color to a mapped value.
        void set_mapped(std::uint32_t mv);

        std::byte*             m_ptr; // A pointer to the current color.
        const SDL_PixelFormat* m_fmt;
    };

    // A builder pattern drawing proxy for surfaces.
    class blitter : public detail::drawer<surface, sdl::pixel_t, const surface, blitter>
    {
    public:
        using drawer::drawer;

        void operator()();

        // SDL's blitting function overwrites the destination rectangle.
        // This overload creates a copy to ensure it remains unchanged.
        void operator()(HAL_TAG_NAME(keep_dst)) const;
    };
}