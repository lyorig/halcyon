#pragma once

#include <halcyon/internal/drawer.hpp>
#include <halcyon/internal/iostream.hpp>
#include <halcyon/internal/pixel_reference.hpp>
#include <halcyon/internal/resource.hpp>

#include <halcyon/types/color.hpp>
#include <halcyon/types/result.hpp>

#include "SDL3/SDL_surface.h"

#include <span>

// surface.hpp:
// A software bitmap, from which textures are created.
// Offers fast pixel retrieval and manipulation, which textures do not,
// but surface-on-surface blitting is slower if GPU acceleration is availible.

struct SDL_Window;

namespace hal
{
    // Forward definitions for helper classes.
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

    class surface : public detail::resource<SDL_Surface, ::SDL_DestroySurface>
    {
    public:
        consteval static pixel::format default_pixel_format()
        {
            return pixel::format::rgba32;
        }

        surface() = default;

        surface(SDL_Surface*);
        surface(std::nullptr_t) = delete;

        // Create a sized surface with an optional pixel format.
        surface(pixel::point sz, pixel::format fmt = default_pixel_format());

        // Load a BMP image. This works natively without having to initialize anything.
        surface(accessor src);

        surface(const surface& s);
        surface& operator=(const surface& s);

        surface(surface&&)            = default;
        surface& operator=(surface&&) = default;

        // Fill the entire surface with a color.
        outcome fill(color clr);

        // Fill a rectangle with a color.
        outcome fill(pixel::rect area, color clr);

        // Fill an array of rectangles with a color.
        outcome fill(std::span<const pixel::rect> areas, color clr);

        outcome lock();
        void    unlock();

        // Save this surface in a BMP format.
        // More formats are made available by the image context.
        outcome save(outputter dst) const;

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

        // Whether the surface must be locked before reading/writing pixels.
        bool must_lock() const;

        // Get pixel at position.
        // It is recommended to manipulate pixels at the surface stage,
        // as textures are very slow to retrieve pixel information.
        pixel::const_reference operator[](pixel::point pos) const;
        pixel::reference       operator[](pixel::point pt);

        // SDL surfaces are not pimpl'd and can be accessed directly.
        SDL_Surface& operator*() const;

        SDL_Surface* operator->() const;

        // Get/set this surface's blend mode.
        result<blend_mode> blend() const;
        outcome            blend(blend_mode bm);

        // Get/set this surface's color modifiers.
        result<color> color_mod() const;
        outcome       color_mod(color col);

        // Get/set this surface's alpha modifier.
        result<color::value_t> alpha_mod() const;
        outcome                alpha_mod(color::value_t val);
    };

    HAL_TAG(keep_dst);

    // A builder pattern drawing proxy for surfaces.
    class blitter : public detail::drawer<const surface, pixel_t, pixel_t, surface, blitter>
    {
    public:
        using drawer::drawer;

        // Finish the operation.
        // SDL's blitting function overwrites the destination rectangle.
        // If you with to reuse this object, use the "keep_dst" overload.
        outcome operator()();

        // Finish the operation.
        // SDL's blitting function overwrites the destination rectangle.
        // This overload uses a copy to ensure it remains unchanged.
        outcome operator()(HAL_TAG_NAME(keep_dst)) const;
    };
}
