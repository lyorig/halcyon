#include <halcyon/video/texture.hpp>

#include <halcyon/debug.hpp>
#include <halcyon/surface.hpp>
#include <halcyon/video/renderer.hpp>

using namespace hal;

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

static_texture::static_texture(clref<renderer> rnd, ref<const surface> surf)
    : texture { ::SDL_CreateTextureFromSurface(rnd->get(), surf->get()) }
{
}

target_texture::target_texture(clref<renderer> rnd, pixel::point size, pixel::format fmt)
    : texture { ::SDL_CreateTexture(rnd->get(), static_cast<Uint32>(fmt), SDL_TEXTUREACCESS_TARGET, size.x, size.y) }
{
}

streaming_texture::streaming_texture(clref<renderer> rnd, pixel::point size, pixel::format fmt)
    : texture { ::SDL_CreateTexture(rnd->get(), static_cast<Uint32>(fmt), SDL_TEXTUREACCESS_STREAMING, size.x, size.y) }
{
}

streaming_texture::data streaming_texture::lock(pixel::rect area)
{
    data ret;

    HAL_ASSERT_VITAL(::SDL_LockTexture(get(), area.addr(), reinterpret_cast<void**>(&ret.pixels), &ret.pitch) == 0, debug::last_error());

    return ret;
}

void streaming_texture::unlock()
{
    ::SDL_UnlockTexture(get());
}
