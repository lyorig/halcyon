#include <halcyon/video/texture.hpp>

#include <halcyon/debug.hpp>
#include <halcyon/surface.hpp>
#include <halcyon/video/renderer.hpp>

using namespace hal;

texture::texture(SDL_Texture* ptr)
    : resource { ptr }
{
}

texture::texture(lref<const renderer> rnd, pixel::format fmt, access a, pixel::point size)
    : texture { ::SDL_CreateTexture(rnd.get(), static_cast<Uint32>(fmt), static_cast<int>(a), size.x, size.y) }
{
}

pixel::point texture::size() const
{
    point<int> size;

    this->query(nullptr, nullptr, &size.x, &size.y);

    return size;
}

color::value_t texture::alpha_mod() const
{
    color::value_t ret;

    ::SDL_GetTextureAlphaMod(get(), &ret);

    return ret;
}

void texture::alpha_mod(color::value_t val)
{
    ::SDL_SetTextureAlphaMod(get(), val);
}

color texture::color_mod() const
{
    color c;

    HAL_ASSERT_VITAL(::SDL_GetTextureColorMod(get(), &c.r, &c.g, &c.b) == 0, debug::last_error());

    return c;
}

void texture::color_mod(color clr)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureColorMod(get(), clr.r, clr.g, clr.b) == 0, debug::last_error());
}

blend_mode texture::blend() const
{
    SDL_BlendMode bm;

    HAL_ASSERT_VITAL(::SDL_GetTextureBlendMode(get(), &bm) == 0, debug::last_error());

    return blend_mode(bm);
}

void texture::blend(blend_mode bm)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureBlendMode(get(), static_cast<SDL_BlendMode>(bm)) == 0, debug::last_error());
}

pixel::format texture::pixel_format() const
{
    Uint32 ret;

    query(&ret, nullptr, nullptr, nullptr);

    return static_cast<pixel::format>(ret);
}

void texture::query(Uint32* format, int* access, int* w, int* h) const
{
    HAL_ASSERT_VITAL(::SDL_QueryTexture(get(), format, access, w, h) == 0, debug::last_error());
}

static_texture::static_texture(lref<const renderer> rnd, pixel::point size, pixel::format fmt)
    : texture { rnd, fmt, access::static_, size }
{
}

static_texture::static_texture(lref<const renderer> rnd, ref<const surface> surf)
    : texture { ::SDL_CreateTextureFromSurface(rnd.get(), surf.get()) }
{
}

void static_texture::update(ref<const surface> surf, pixel::point pos)
{
    update(surf, { pos, surf->size() });
}

void static_texture::update(ref<const surface> surf, pixel::rect area)
{
    HAL_ASSERT(surf->size() >= area.size, "The surface must be >= to the area");

    common_update(area.addr(), surf.get()->pixels, surf.get()->pitch);
}

void static_texture::common_update(const SDL_Rect* area, const void* pixels, int pitch)
{
    HAL_ASSERT_VITAL(::SDL_UpdateTexture(get(), area, pixels, pitch) == 0, debug::last_error());
}

target_texture::target_texture(lref<const renderer> rnd, pixel::point size, pixel::format fmt)
    : texture { rnd, fmt, access::target, size }
{
}

streaming_texture::streaming_texture(lref<const renderer> rnd, pixel::point size, pixel::format fmt)
    : texture { rnd, fmt, access::streaming, size }
{
}

streaming_texture::data streaming_texture::lock()
{
    return common_lock(nullptr);
}

streaming_texture::data streaming_texture::lock(pixel::rect area)
{
    return common_lock(area.addr());
}

streaming_texture::data streaming_texture::common_lock(const SDL_Rect* area)
{
    data ret;

    HAL_ASSERT_VITAL(::SDL_LockTexture(get(), area, reinterpret_cast<void**>(&ret.pixels), &ret.pitch) == 0, debug::last_error());

    return ret;
}

void streaming_texture::unlock()
{
    ::SDL_UnlockTexture(get());
}
