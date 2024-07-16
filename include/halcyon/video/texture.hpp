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
    class texture;

    template <>
    class view<const texture> : public detail::view_base<SDL_Texture>
    {
    protected:
        using view_base::view_base;

    public:
        pixel::point size() const;

        color::value_t opacity() const;

        color::value_t alpha_mod() const;

        color color_mod() const;

        blend_mode blend() const;

        pixel::format pixel_format() const;

    private:
        void query(Uint32* format, int* access, int* w, int* h) const;
    };

    template <>
    class view<texture> : public view<const texture>
    {
        using super = view<const texture>;

    protected:
        using super::super;

    public:
        using super::opacity;
        void opacity(color::value_t value);

        using super::alpha_mod;
        void alpha_mod(color::value_t val);

        using super::color_mod;
        void color_mod(color mod);

        using super::blend;
        void blend(blend_mode bm);
    };

    // Common texture functionality.
    class texture : public detail::raii_object<texture, &::SDL_DestroyTexture>
    {
    protected:
        texture() = default;

        texture(SDL_Texture* ptr);
    };

    // Forward declarations for parameters and return types.
    class renderer;

    // A texture that cannot be drawn onto, only reassigned.
    class static_texture : public texture
    {
    public:
        static_texture() = default;

        static_texture(view<const renderer> rnd, view<const surface> surf);
    };

    // A texture that can be drawn onto.
    class target_texture : public texture
    {
    public:
        target_texture() = default;

        target_texture(view<const renderer> rnd, pixel::format fmt, pixel::point size);
    };

    class streaming_texture : public texture
    {
    public:
        streaming_texture() = default;

        streaming_texture(view<const renderer> rnd, pixel::format fmt, pixel::point size);
    };
}
