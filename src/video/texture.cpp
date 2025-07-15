#include <halcyon/video/texture.hpp>

#include <halcyon/debug.hpp>
#include <halcyon/surface.hpp>

#include <halcyon/video/renderer.hpp>

using namespace hal;

texture::texture(lref<const renderer> rnd, pixel::format fmt, access a, pixel::point size)
    : resource { ::SDL_CreateTexture(rnd.get(), static_cast<SDL_PixelFormat>(fmt), static_cast<SDL_TextureAccess>(a), size.x, size.y) }
{
}

result<pixel::point> texture::size() const
{
    coord::point size;

    return { ::SDL_GetTextureSize(get(), &size.x, &size.y), size };
}

result<color::value_t> texture::alpha_mod() const
{
    color::value_t ret;
    return { ::SDL_GetTextureAlphaMod(get(), &ret), ret };
}

bool texture::alpha_mod(color::value_t val)
{
    return ::SDL_SetTextureAlphaMod(get(), val);
}

result<color> texture::color_mod() const
{
    color c;

    return { ::SDL_GetTextureColorMod(get(), &c.r, &c.g, &c.b), c };
}

bool texture::color_mod(color clr)
{
    return ::SDL_SetTextureColorMod(get(), clr.r, clr.g, clr.b);
}

result<blend_mode> texture::blend() const
{
    SDL_BlendMode bm;
    return { ::SDL_GetTextureBlendMode(get(), &bm), static_cast<blend_mode>(bm) };
}

bool texture::blend(blend_mode bm)
{
    return ::SDL_SetTextureBlendMode(get(), static_cast<SDL_BlendMode>(bm));
}

result<pixel::format> texture::pixel_format() const
{
    return { true, static_cast<pixel::format>(::SDL_GetNumberProperty(::SDL_GetTextureProperties(get()), "SDL_PROP_TEXTURE_FORMAT_NUMBER", 0)) };
}

static_texture::static_texture(lref<const renderer> rnd, pixel::point size, pixel::format fmt)
    : texture { rnd, fmt, access::static_, size }
{
}

static_texture::static_texture(lref<const renderer> rnd, ref<const surface> surf)
    : texture { ::SDL_CreateTextureFromSurface(rnd.get(), surf.get()) }
{
}

bool static_texture::update(ref<const surface> surf, pixel::point pos)
{
    return update(surf, { pos, surf->size() });
}

bool static_texture::update(ref<const surface> surf, pixel::rect area)
{
    HAL_ASSERT(surf->size() >= area.size, "The surface must be >= to the area");

    return internal_update(area.sdl_ptr(), surf.get()->pixels, surf.get()->pitch);
}

bool static_texture::internal_update(const SDL_Rect* area, const void* pixels, int pitch)
{
    return ::SDL_UpdateTexture(get(), area, pixels, pitch);
}

target_texture::target_texture(lref<const renderer> rnd, pixel::point size, pixel::format fmt)
    : texture { rnd, fmt, access::target, size }
{
}

streaming_texture::streaming_texture(lref<const renderer> rnd, pixel::point size, pixel::format fmt)
    : texture { rnd, fmt, access::streaming, size }
{
}

result<lock_data> streaming_texture::lock()
{
    return internal_lock(nullptr);
}

result<lock_data> streaming_texture::lock(pixel::rect area)
{
    return internal_lock(area.sdl_ptr());
}

result<lock_data> streaming_texture::internal_lock(const SDL_Rect* area)
{
    lock_data ret;

    return { ::SDL_LockTexture(get(), area, reinterpret_cast<void**>(&ret.pixels), &ret.pitch), ret };
}

void streaming_texture::unlock()
{
    ::SDL_UnlockTexture(get());
}
