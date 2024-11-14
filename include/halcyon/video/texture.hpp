#pragma once

#include <halcyon/internal/resource.hpp>
#include <halcyon/types/color.hpp>
#include <halcyon/types/result.hpp>
#include <halcyon/video/types.hpp>

#include "SDL_render.h"

// video/texture.cpp:
// Proper textures that can be drawn to a window (or a target texture).

namespace hal
{
    class surface;

    // Base texture class. Do not create directly.
    //
    class texture : public detail::resource<SDL_Texture, &::SDL_DestroyTexture>
    {
    protected:
        // A special integer value that tells SDL to do the thing it's named after when creating a texture.
        constexpr static pixel::format use_renderer_native()
        {
            return static_cast<pixel::format>(0);
        }

        enum class access : u8
        {
            static_   = SDL_TEXTUREACCESS_STATIC,
            target    = SDL_TEXTUREACCESS_TARGET,
            streaming = SDL_TEXTUREACCESS_STREAMING
        };

        texture() = default;

        texture(SDL_Texture* ptr);

        texture(lref<const renderer> rnd, pixel::format fmt, access a, pixel::point size);

    public:
        pixel::point size() const;

        result<color::value_t> alpha_mod() const;
        outcome                alpha_mod(color::value_t val);

        result<color> color_mod() const;
        outcome       color_mod(color mod);

        result<blend_mode> blend() const;
        outcome            blend(blend_mode bm);

        result<pixel::format> pixel_format() const;

    private:
        outcome internal_query(Uint32* format, int* access, int* w, int* h) const;
    };

    // Forward declarations for parameters and return types.
    class renderer;

    // A texture that cannot be drawn onto or otherwise manipulated.
    // Use if you don't intend to change it often.
    class static_texture : public texture
    {
    public:
        static_texture() = default;

        static_texture(lref<const renderer> rnd, pixel::point size, pixel::format fmt = use_renderer_native());
        static_texture(lref<const renderer> rnd, ref<const surface> surf);

        // Update the texture with pixels of a surface. The surface's pixel format must match the texture's.
        outcome update(ref<const surface> surf, pixel::point pos = { 0, 0 });

        // Update the texture with pixels of a surface and stretch them to a certain area.
        // The surface's pixel format must match the texture's, and its size must be >= the area's.
        outcome update(ref<const surface> surf, pixel::rect area);

    private:
        outcome internal_update(const SDL_Rect* area, const void* pixels, int pitch);
    };

    // A texture that can be drawn onto.
    // Use if you want to draw textures onto it.
    class target_texture : public texture
    {
    public:
        target_texture() = default;

        target_texture(lref<const renderer> rnd, pixel::point size, pixel::format fmt = use_renderer_native());
    };

    // A texture whose pixels can be accessed.
    // Use if you want to directly manipulate pixels.
    class streaming_texture : public texture
    {
    public:
        streaming_texture() = default;

        streaming_texture(lref<const renderer> rnd, pixel::point size, pixel::format fmt = use_renderer_native());

        struct data
        {
            std::byte* pixels;
            int        pitch;
        };

        result<data> lock();
        result<data> lock(pixel::rect area);

        void unlock();

    private:
        result<data> internal_lock(const SDL_Rect* area);
    };
}
