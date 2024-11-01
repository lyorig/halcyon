#include <halcyon/surface.hpp>

#include <halcyon/utility/guard.hpp>

#include "SDL_image.h"

using namespace hal;

namespace
{
    // Convert a color to a mapped value using a surface's pixel format.
    Uint32 mapped(const SDL_PixelFormat* fmt, color c)
    {
        return ::SDL_MapRGBA(fmt, c.r, c.g, c.b, c.a);
    }
}

surface::surface(pixel::point sz, pixel::format fmt)
    : resource { ::SDL_CreateRGBSurfaceWithFormat(0, sz.x, sz.y, CHAR_BIT * 4, static_cast<Uint32>(fmt)) }
{
}

surface::surface(accessor src)
    : resource { ::SDL_LoadBMP_RW(src.use(pass_key<surface> {}), true) }
{
}

surface::surface(SDL_Surface* ptr, pass_key<surface>)
    : resource { ptr }
{
}

surface::surface(SDL_Surface* ptr, pass_key<image::context>)
    : resource { ptr }
{
}

surface::surface(SDL_Surface* ptr, pass_key<builder::font_text>)
    : resource { ptr }
{
}

surface::surface(SDL_Surface* ptr, pass_key<builder::font_glyph>)
    : resource { ptr }
{
}

outcome surface::fill(color clr)
{
    return ::SDL_FillRect(get(), nullptr, mapped(get()->format, clr));
}

outcome surface::fill(pixel::rect area, color clr)
{
    return ::SDL_FillRect(get(), area.addr(), mapped(get()->format, clr));
}

outcome surface::fill(std::span<const pixel::rect> areas, color clr)
{
    return ::SDL_FillRects(get(), reinterpret_cast<const SDL_Rect*>(areas.data()), static_cast<int>(areas.size()), mapped(get()->format, clr));
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
    return ::SDL_SaveBMP_RW(get(), dst.use(pass_key<surface> {}), true);
}

blitter surface::blit(surface& dst) const
{
    return { dst, *this };
}

surface surface::convert(pixel::format fmt) const
{
    return { ::SDL_ConvertSurfaceFormat(get(), static_cast<Uint32>(fmt), 0), pass_key<surface> {} };
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
    return static_cast<pixel::format>(get()->format->format);
}

surface surface::resize(pixel::point sz) const
{
    surface ret { sz };

    blit(ret).to(tag::fill)();

    return ret;
}

surface surface::resize(scaler scl) const
{
    return resize(scl(size()));
}

bool surface::must_lock() const
{
    return SDL_MUSTLOCK(get());
}

const_pixel_reference surface::operator[](pixel::point pos) const
{
    HAL_ASSERT(pos.x < get()->w, "Out-of-range width");
    HAL_ASSERT(pos.y < get()->h, "Out-of-range height");

    return { static_cast<std::byte*>(get()->pixels), get()->pitch, get()->format, pos, pass_key<surface> {} };
}

pixel_reference surface::operator[](pixel::point pos)
{
    return { static_cast<std::byte*>(get()->pixels), get()->pitch, get()->format, pos, pass_key<surface> {} };
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
    return ::SDL_SetSurfaceColorMod(get(), col.r, col.g, col.b) == 0;
}

result<color::value_t> surface::alpha_mod() const
{
    color::value_t ret;

    return { ::SDL_GetSurfaceAlphaMod(get(), &ret) == 0, ret };
}

outcome surface::alpha_mod(color::value_t val)
{
    return ::SDL_SetSurfaceAlphaMod(get(), val);
}

// Const pixel reference.

const_pixel_reference::const_pixel_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos, pass_key<surface>)
    : const_pixel_reference { pixels, pitch, fmt, pos }
{
}

const_pixel_reference::const_pixel_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos)
    : m_ptr { pixels + pos.y * pitch + pos.x * fmt->BytesPerPixel }
    , m_fmt { fmt }
{
}

color const_pixel_reference::color() const
{
    hal::color c;

    ::SDL_GetRGBA(get_mapped(), m_fmt, &c.r, &c.g, &c.b, &c.a);

    return c;
}

Uint32 const_pixel_reference::get_mapped() const
{
    Uint32 ret { 0 };

    if constexpr (compile_settings::endianness == endian::lil)
    {
        std::memcpy(&ret, m_ptr, m_fmt->BytesPerPixel);
    }

    else
    {
        const u8 offset { static_cast<u8>(sizeof(Uint32) - m_fmt->BytesPerPixel) };
        std::memcpy(&ret + offset, m_ptr + offset, m_fmt->BytesPerPixel);
    }

    return ret;
}

// Pixel reference.

pixel_reference::pixel_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos, pass_key<surface>)
    : const_pixel_reference(pixels, pitch, fmt, pos)
{
}

void pixel_reference::color(struct color c)
{
    set_mapped(::SDL_MapRGBA(m_fmt, c.r, c.g, c.b, c.a));
}

void pixel_reference::set_mapped(Uint32 mapped)
{
    if constexpr (compile_settings::endianness == endian::lil)
    {
        std::memcpy(m_ptr, &mapped, m_fmt->BytesPerPixel);
    }

    else
    {
        const u8 offset = sizeof(Uint32) - m_fmt->BytesPerPixel;
        std::memcpy(m_ptr + offset, &mapped + offset, m_fmt->BytesPerPixel);
    }
}

// Blitter.

outcome blitter::operator()()
{
    return ::SDL_BlitScaled(
        m_this.get(),
        m_src.pos.x == unset_pos<src_t>() ? nullptr : reinterpret_cast<const SDL_Rect*>(m_src.addr()),
        m_pass.get(),
        m_dst.pos.x == unset_pos<dst_t>() ? nullptr : reinterpret_cast<SDL_Rect*>(m_dst.addr()));
}

outcome blitter::operator()(HAL_TAG_NAME(keep_dst)) const
{
    pixel::rect copy { m_dst };

    return ::SDL_BlitScaled(
        m_this.get(),
        m_src.pos.x == unset_pos<src_t>() ? nullptr : m_src.addr(),
        m_pass.get(),
        m_dst.pos.x == unset_pos<dst_t>() ? nullptr : copy.addr());
}
