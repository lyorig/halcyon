#pragma once

#include <SDL2/SDL_surface.h>

#include <halcyon/enums/anchor.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>

#include "sdl_object.hpp"

/* surface.hpp:
   A "software surface" used by SDL. The user should never
   directly create a surface, but instead use various loading
   functions of other classes, which are responsible for providing
   a valid SDL_Surface pointer. On top of staying true to the SDL object
   structure, another upside comes in the form of early error handling. */

namespace hal {
class image_loader;
class window;
class font;

class surface : public sdl_object<SDL_Surface, &::SDL_FreeSurface> {
public:
    // Create a sized surface.
    surface(const window& wnd, pixel_size sz);

    // Get a resized copy of the surface. Useful for saving
    // memory after converting to a texture.
    surface resize(pixel_size sz) const;

    // Get a scaled copy of the surface. Useful for saving
    // memory after converting to a texture.
    surface resize(lyo::f64 scale) const;

    void set_blend(SDL_BlendMode bm) const;

    pixel_size size() const;

    // Get pixel at position.
    // This functionality is exclusive to surfaces, as textures
    // are extremely slow to retrieve pixel information.
    color operator[](pixel_pos coord) const;

    class draw {
    public:
        [[nodiscard]] draw(const surface& src);

        // Set where to draw the surface.
        [[nodiscard]] draw& to(const pixel_pos& pos);

        // Set the surface's destination rectangle.
        [[nodiscard]] draw& to(const pixel_area& area);

        // Stretch the surface across the destination surface.
        [[nodiscard]] draw& to(fill_tag);

        // Set the surface's source rectangle.
        [[nodiscard]] draw& from(const pixel_area& area);

        // Scale the surface's destination size.
        [[nodiscard]] draw& scale(lyo::f64 mul);

        // Scale the surface's destination size.
        [[nodiscard]] draw& anchor(anchor::pos anch);

        void operator()(const surface& dst) const;

    private:
        const surface& m_this;

        rect_wrap m_src;

        // SDL wants to output the drawn size in the destination
        // rectangle. We don't care about this, but the function
        // should be const. Thus, mutable.
        mutable rect_wrap m_dst;
    };

private:
    friend class image_loader;
    friend class font;

    // Special c-tor for factory classes.
    surface(SDL_Surface* surf);

    // Special c-tor for resizing.
    surface(pixel_size sz);

    Uint32 get_pixel(pixel_type x, pixel_type y) const;
};
} // namespace hal