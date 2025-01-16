#pragma once

#include <halcyon/utility/metaprogramming.hpp>

#include "SDL3/SDL_rect.h"

// internal/video_basic_types.hpp:
// This file is separate from video/types.hpp to prevent include errors.

namespace hal
{
    using pixel_t = decltype(SDL_Rect::x);
    using coord_t = decltype(SDL_FRect::x);

    // Paranoia.
    static_assert(std::is_signed_v<pixel_t>);
    static_assert(std::is_signed_v<coord_t>);

    namespace detail
    {
        template <typename T>
        concept pixel_or_coord = meta::one_of<T, pixel_t, coord_t>;
    }
}
