#pragma once

#include <halcyon/types/color.hpp>
#include <halcyon/utility/pass_key.hpp>
#include <halcyon/video/types.hpp>

#include "SDL3/SDL_pixels.h"

#include <cstddef>

namespace hal
{
    class surface;
    class streaming_texture;

    namespace pixel
    {
        class const_reference
        {
        public:
            const_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos, pass_key<surface>);

            color color() const;

        protected:
            const_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos);
            // Retrieve the color in a mapped format.
            Uint32 get_mapped() const;

            std::byte*             m_ptr; // A pointer to the current color.
            const SDL_PixelFormat* m_fmt;
        };

        // A reference to a pixel in a surface for easy access and modification.
        class reference : public const_reference
        {
        public:
            reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos, pass_key<surface>);

            using const_reference::color;
            void color(hal::color c);

        private:
            // Set the color to a mapped value.
            void set_mapped(Uint32 mv);
        };
    }
}
