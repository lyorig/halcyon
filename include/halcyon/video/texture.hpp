#pragma once

#include <span>

#include <SDL_render.h>

#include <halcyon/utility/pass_key.hpp>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/types/color.hpp>
#include <halcyon/video/types.hpp>

// video/texture.cpp:
// Proper textures that can be drawn to a window (or a target texture).

namespace hal
{
    class surface;

    // Common texture functionality.
    class texture : public detail::raii_object<SDL_Texture, &::SDL_DestroyTexture>
    {
    protected:
        texture() = default;

        texture(SDL_Texture* ptr);

    public:
        pixel::point size() const;

        color::value_t alpha_mod() const;
        void           alpha_mod(color::value_t val);

        color color_mod() const;
        void  color_mod(color mod);

        blend_mode blend() const;
        void       blend(blend_mode bm);

        pixel::format pixel_format() const;

    private:
        void query(Uint32* format, int* access, int* w, int* h) const;
    };

    // Forward declarations for parameters and return types.
    class renderer;

    // A texture that cannot be drawn onto, only reassigned.
    class static_texture : public texture
    {
    public:
        static_texture() = default;

        static_texture(ref<renderer> rnd, ref<const surface> surf);
    };

    // A texture that can be drawn onto.
    class target_texture : public texture
    {
    public:
        target_texture() = default;

        target_texture(ref<renderer> rnd, pixel::format fmt, pixel::point size);
    };

    class streaming_texture : public texture
    {
    public:
        streaming_texture() = default;

        streaming_texture(ref<renderer> rnd, pixel::format fmt, pixel::point size);
    };
}
