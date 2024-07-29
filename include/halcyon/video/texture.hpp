#pragma once

#include <SDL_render.h>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/types/color.hpp>
#include <halcyon/video/types.hpp>

#include <halcyon/utility/non_null.hpp>

// video/texture.cpp:
// Proper textures that can be drawn to a window (or a target texture).

namespace hal
{
    class surface;

    // Common texture functionality. Cannot be constructed, instead use static, target, or streaming textures.
    class texture : public detail::raii_object<SDL_Texture, &::SDL_DestroyTexture>
    {
    protected:
        enum class access : u8
        {
            static_   = SDL_TEXTUREACCESS_STATIC,
            target    = SDL_TEXTUREACCESS_TARGET,
            streaming = SDL_TEXTUREACCESS_STREAMING
        };

        texture() = default;

        texture(SDL_Texture* ptr);

        texture(clref<renderer> rnd, pixel::format fmt, access a, pixel::point size);

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

    // A texture that cannot be drawn onto or otherwise manipulated.
    // Use if you don't intend to change it often.
    class static_texture : public texture
    {
    public:
        static_texture() = default;

        static_texture(clref<renderer> rnd, pixel::point size, pixel::format fmt);
        static_texture(clref<renderer> rnd, ref<const surface> surf);

        // Update the texture with your own pixel buffer. These pixels' format
        // must match the texture's.
        void update(non_null<const std::byte> pixel_buffer, int pitch);
        void update(non_null<const std::byte> pixel_buffer, int pitch, pixel::rect area);

        // Update the texture with pixels of a surface. The surface's pixel format must match the texture's.
        void update(ref<const surface> surf, pixel::point pos = { 0, 0 });

        // Update the textzre with pixels of a surface and stretch them to a certain area.
        // The surface's pixel format must match the texture's, and its size must be >= the area's.
        void update(ref<const surface> surf, pixel::rect area);

    private:
        void common_update(const SDL_Rect* area, non_null<const void> pixels, int pitch);
    };

    // A texture that can be drawn onto.
    // Use if you want to draw textures onto it.
    class target_texture : public texture
    {
    public:
        target_texture() = default;

        target_texture(clref<renderer> rnd, pixel::point size, pixel::format fmt);
    };

    // A texture whose pixels can be accessed.
    // Use if you want to directly manipulate pixels.
    class streaming_texture : public texture
    {
    public:
        streaming_texture() = default;

        streaming_texture(clref<renderer> rnd, pixel::point size, pixel::format fmt);

        struct data
        {
            std::byte* pixels;
            int        pitch;
        };

        data lock();
        data lock(pixel::rect area);

        void unlock();

    private:
        data common_lock(const SDL_Rect* area);
    };
}
