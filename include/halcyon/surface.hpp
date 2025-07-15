#pragma once

#include <halcyon/internal/drawer.hpp>
#include <halcyon/internal/iostream.hpp>
#include <halcyon/internal/resource.hpp>

#include <halcyon/types/color.hpp>
#include <halcyon/types/result.hpp>

#include "SDL3/SDL_surface.h"

#include <span>

// surface.hpp:
// A software bitmap, from which textures are created.
// Offers fast pixel retrieval and manipulation, which textures do not,
// but surface-on-surface blitting is slower if GPU acceleration is availible.

namespace hal
{
    // Forward definitions for helper classes.
    class blitter;

    class surface : public detail::resource<SDL_Surface, ::SDL_DestroySurface>
    {
    public:
        consteval static pixel::format default_pixel_format()
        {
            return pixel::format::rgba32;
        }

        consteval static scale_mode default_scale_mode()
        {
            return scale_mode::nearest;
        }

        surface() = default;

        surface(pointer ptr);

        // Create a sized surface with an optional pixel format.
        surface(pixel::point sz, pixel::format fmt = default_pixel_format());

        // Load a BMP image. This works natively without having to initialize anything.
        surface(accessor src);

        surface(const surface& s);
        surface& operator=(const surface& s);

        surface(surface&&)            = default;
        surface& operator=(surface&&) = default;

        // Fill the entire surface with a color.
        bool fill(color clr);

        // Fill a rectangle with a color.
        bool fill(pixel::rect area, color clr);

        // Fill an array of rectangles with a color.
        bool fill(std::span<const pixel::rect> areas, color clr);

        bool lock();
        void unlock();

        // Save this surface in a BMP format.
        // More formats are made available by the image context.
        bool save(outputter dst) const;

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
        surface resize(pixel::point sz, scale_mode sm = default_scale_mode()) const;

        // Whether the surface must be locked before reading/writing pixels.
        bool must_lock() const;

        // Get pixel at position.
        // It is recommended to manipulate pixels at the surface stage,
        // as textures are very slow to retrieve pixel information.
        result<color> pixel(pixel::point pos) const;
        bool          pixel(pixel::point pos, color c);

        // SDL surfaces are not pimpl'd and can be accessed directly.
        SDL_Surface& operator*() const;

        SDL_Surface* operator->() const;

        // Get/set this surface's blend mode.
        result<blend_mode> blend() const;
        bool               blend(blend_mode bm);

        // Get/set this surface's color modifiers.
        result<color> color_mod() const;
        bool          color_mod(color col);

        // Get/set this surface's alpha modifier.
        result<color::value_t> alpha_mod() const;
        bool                   alpha_mod(color::value_t val);

    private:
        Uint32 map_rgb(color c) const;
        Uint32 map_rgba(color c) const;
    };

    HAL_TAG(keep_dst);

    // A builder pattern drawing proxy for surfaces.
    class blitter : public detail::drawer<const surface, surface, pixel_t, blitter>
    {
    public:
        using drawer::drawer;

        bool blit() const;
        bool scaled(scale_mode sm) const;
        bool tiled() const;
        bool tiled_scale(float scale, scale_mode sm) const;
        bool nine_grid(pixel_t width_left, pixel_t width_right, pixel_t height_top, pixel_t height_bottom, float scale, scale_mode sm) const;
    };
}
