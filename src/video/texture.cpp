#include <halcyon/video/texture.hpp>

#include <halcyon/debug.hpp>
#include <halcyon/surface.hpp>
#include <halcyon/video/renderer.hpp>

using namespace hal;

using cv = view<const texture>;

pixel::point cv::size() const
{
    point<int> size;

    this->query(nullptr, nullptr, &size.x, &size.y);

    return size;
}

color::value_t cv::alpha_mod() const
{
    color::value_t ret;

    ::SDL_GetTextureAlphaMod(get(), &ret);

    return ret;
}

color cv::color_mod() const
{
    color c;

    HAL_ASSERT_VITAL(::SDL_GetTextureColorMod(get(), &c.r, &c.g, &c.b) == 0, debug::last_error());

    return c;
}

blend_mode cv::blend() const
{
    SDL_BlendMode bm;

    HAL_ASSERT_VITAL(::SDL_GetTextureBlendMode(get(), &bm) == 0, debug::last_error());

    return blend_mode(bm);
}

pixel::format cv::pixel_format() const
{
    Uint32 ret;

    query(&ret, nullptr, nullptr, nullptr);

    return static_cast<pixel::format>(ret);
}

u8 cv::opacity() const
{
    Uint8 alpha;

    HAL_ASSERT_VITAL(::SDL_GetTextureAlphaMod(get(), &alpha) == 0,
        debug::last_error());

    return alpha;
}

void cv::query(Uint32* format, int* access, int* w, int* h) const
{
    HAL_ASSERT_VITAL(::SDL_QueryTexture(get(), format, access, w, h) == 0, debug::last_error());
}

using v = view<texture>;

void v::opacity(color::value_t value)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureAlphaMod(get(), value) == 0, debug::last_error());
}

void v::alpha_mod(color::value_t val)
{
    ::SDL_SetTextureAlphaMod(get(), val);
}

void v::color_mod(color clr)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureColorMod(get(), clr.r, clr.g, clr.b) == 0, debug::last_error());
}

void v::blend(blend_mode bm)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureBlendMode(get(), static_cast<SDL_BlendMode>(bm)) == 0, debug::last_error());
}

texture::texture(SDL_Texture* ptr)
    : raii_object { ptr }
{
    this->blend(blend_mode::blend);
}

static_texture::static_texture(view<const renderer> rnd, view<const surface> surf)
    : texture { ::SDL_CreateTextureFromSurface(rnd.get(), surf.get()) }
{
}

target_texture::target_texture(view<const renderer> rnd, pixel::format fmt, pixel::point size)
    : texture { ::SDL_CreateTexture(rnd.get(), static_cast<Uint32>(fmt), SDL_TEXTUREACCESS_TARGET, size.x, size.y) }
{
}

streaming_texture::streaming_texture(view<const renderer> rnd, pixel::format fmt, pixel::point size)
    : texture { ::SDL_CreateTexture(rnd.get(), static_cast<Uint32>(fmt), SDL_TEXTUREACCESS_STREAMING, size.x, size.y) }
{
}
