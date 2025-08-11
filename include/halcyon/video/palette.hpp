#pragma once

#include <halcyon/internal/resource.hpp>
#include <halcyon/types/color.hpp>
#include <span>

#include "SDL3/SDL_pixels.h"

namespace hal
{
    class palette : public detail::resource<SDL_Palette, ::SDL_DestroyPalette>
    {
    public:
        palette(int n_colors);

        // Set this palette's colors.
        bool colors(std::span<const color> c);
    };
}
