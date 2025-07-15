#include <halcyon/surface.hpp>

#include <halcyon/utility/guard.hpp>

using namespace hal;

namespace
{
    SDL_Surface* copy_from(ref<const surface> s)
    {
        const SDL_Surface& ref { *s->get() };
        return ::SDL_CreateSurfaceFrom(ref.w, ref.h, ref.format, ref.pixels, ref.pitch);
    }
}

surface::surface(pointer ptr)
    : resource { ptr }
{
}

surface::surface(pixel::point sz, pixel::format fmt)
    : resource { ::SDL_CreateSurface(sz.x, sz.y, static_cast<SDL_PixelFormat>(fmt)) }
{
}

surface::surface(accessor src)
    : resource { ::SDL_LoadBMP_IO(src.get(), false) }
{
}

surface::surface(const surface& s)
    : resource { copy_from(s) }
{
}

surface& surface::operator=(const surface& s)
{
    reset(copy_from(s));

    return *this;
}

outcome surface::fill(color clr)
{
    return ::SDL_FillSurfaceRect(get(), nullptr, map_rgba(clr));
}

outcome surface::fill(pixel::rect area, color clr)
{
    return ::SDL_FillSurfaceRect(get(), area.sdl_ptr(), map_rgba(clr));
}

outcome surface::fill(std::span<const pixel::rect> areas, color clr)
{
    return ::SDL_FillSurfaceRects(get(), reinterpret_cast<const SDL_Rect*>(areas.data()), static_cast<int>(areas.size()), map_rgba(clr));
}

outcome surface::lock()
{
    return ::SDL_LockSurface(get());
}

void surface::unlock()
{
    ::SDL_UnlockSurface(get());
}

outcome surface::save(outputter dst) const
{
    return ::SDL_SaveBMP_IO(get(), dst.get(), false);
}

blitter surface::blit(surface& dst) const
{
    return { dst, *this };
}

surface surface::convert(pixel::format fmt) const
{
    return ::SDL_ConvertSurface(get(), static_cast<SDL_PixelFormat>(fmt));
}

pixel::point surface::size() const
{
    return {
        pixel_t(get()->w),
        pixel_t(get()->h)
    };
}

pixel::format surface::pixel_format() const
{
    return static_cast<pixel::format>(get()->format);
}

surface surface::resize(pixel::point sz, scale_mode sm) const
{
    surface ret { sz };

    blit(ret).to(tag::fill).scaled(sm);

    return ret;
}

bool surface::must_lock() const
{
    return SDL_MUSTLOCK(get());
}

result<color> surface::pixel(pixel::point pos) const
{
    color ret;
    return { ::SDL_ReadSurfacePixel(get(), pos.x, pos.y, &ret.r, &ret.g, &ret.b, &ret.a), ret };
}

bool surface::pixel(pixel::point pos, color c)
{
    return ::SDL_WriteSurfacePixel(get(), pos.x, pos.y, c.r, c.g, c.b, c.a);
}

SDL_Surface& surface::operator*() const
{
    return *get();
}

SDL_Surface* surface::operator->() const
{
    return get();
}

result<blend_mode> surface::blend() const
{
    SDL_BlendMode bm;

    return { ::SDL_GetSurfaceBlendMode(get(), &bm), static_cast<blend_mode>(bm) };
}

outcome surface::blend(blend_mode bm)
{
    return ::SDL_SetSurfaceBlendMode(get(), SDL_BlendMode(bm));
}

result<color> surface::color_mod() const
{
    color ret;

    return { ::SDL_GetSurfaceColorMod(get(), &ret.r, &ret.g, &ret.b), ret };
}

outcome surface::color_mod(color col)
{
    return ::SDL_SetSurfaceColorMod(get(), col.r, col.g, col.b);
}

result<color::value_t> surface::alpha_mod() const
{
    color::value_t ret;

    return { ::SDL_GetSurfaceAlphaMod(get(), &ret), ret };
}

outcome surface::alpha_mod(color::value_t val)
{
    return ::SDL_SetSurfaceAlphaMod(get(), val);
}

Uint32 surface::map_rgb(color c) const
{
    return ::SDL_MapSurfaceRGB(get(), c.r, c.g, c.b);
}

Uint32 surface::map_rgba(color c) const
{
    return ::SDL_MapSurfaceRGBA(get(), c.r, c.g, c.b, c.a);
}

// Blitter.

bool blitter::blit() const
{
    return ::SDL_BlitSurface(m_drawSrc.get(), m_posSrc.sdl_ptr(), m_drawDst.get(), m_posDst.sdl_ptr());
}

bool blitter::scaled(scale_mode sm) const
{
    return ::SDL_BlitSurfaceScaled(m_drawSrc.get(), m_posSrc.sdl_ptr(), m_drawDst.get(), m_posDst.sdl_ptr(), static_cast<SDL_ScaleMode>(sm));
}

bool blitter::tiled() const
{
    return ::SDL_BlitSurfaceTiled(m_drawSrc.get(), m_posSrc.sdl_ptr(), m_drawDst.get(), m_posDst.sdl_ptr());
}

bool blitter::tiled_scale(float scale, scale_mode sm) const
{
    return ::SDL_BlitSurfaceTiledWithScale(m_drawSrc.get(), m_posSrc.sdl_ptr(), scale, static_cast<SDL_ScaleMode>(sm), m_drawDst.get(), m_posDst.sdl_ptr());
}

bool blitter::nine_grid(pixel_t width_left, pixel_t width_right, pixel_t height_top, pixel_t height_bottom, float scale, scale_mode sm) const
{
    return ::SDL_BlitSurface9Grid(m_drawSrc.get(), m_posSrc.sdl_ptr(), width_left, width_right, height_top, height_bottom, scale, static_cast<SDL_ScaleMode>(sm), m_drawDst.get(), m_posDst.sdl_ptr());
}
