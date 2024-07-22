#pragma once

#include <SDL_render.h>

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
        using raii_object::raii_object;

    public:
        static constexpr pixel::format default_pixel_format { pixel::format::rgba32 };

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

        static_texture(clref<renderer> rnd, ref<const surface> surf);
    };

    // A texture that can be drawn onto.
    class target_texture : public texture
    {
    public:
        target_texture() = default;

        target_texture(clref<renderer> rnd, pixel::point size, pixel::format fmt = texture::default_pixel_format);
    };

    class streaming_texture : public texture
    {
    public:
        streaming_texture() = default;

        streaming_texture(clref<renderer> rnd, pixel::point size, pixel::format fmt = texture::default_pixel_format);

        struct data
        {
            std::byte* pixels;
            int        pitch;
        };

        data lock(pixel::rect area);
        void unlock();
    };
}
