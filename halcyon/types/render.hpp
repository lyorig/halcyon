#pragma once

#include <halcyon/types/slider.hpp>
#include <lyo/types.hpp>

#include <halcyon/internal/SDL_types.hpp>
#include <halcyon/types/point.hpp>
#include <halcyon/types/rectangle.hpp>

/* render.hpp:
   Rendering-related types used throughout Halcyon. */

namespace hal
{
    using pixel_type = lyo::i16;

    using pixel_size = point<pixel_type>;
    using pixel_pos = point<pixel_type>;
    using pixel_area = rectangle<pixel_type>;

    using coord_type = lyo::f64;

    using coord = point<coord_type>;
    using coord_area = rectangle<coord_type>;

    using opacity_slider = static_slider<lyo::f64, SDL_ALPHA_TRANSPARENT, SDL_ALPHA_OPAQUE>;
}

// Paranoia.
static_assert(std::is_signed_v<hal::pixel_type>, "Pixel type must be signed");
static_assert(std::is_floating_point_v<hal::coord_type>, "Position type must be floating point");

static_assert(sizeof(hal::SDL::Point) == sizeof(SDL_Point) && sizeof(hal::SDL::FPoint) == sizeof(SDL_FPoint) && sizeof(hal::SDL::Rect) == sizeof(SDL_Rect) && sizeof(hal::SDL::FRect) == sizeof(SDL_FRect), "Incompatible memory layout between SDL structs and their wrappers");