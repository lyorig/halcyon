#include <halcyon/surface.hpp>

#include <halcyon/utility/guard.hpp>

using namespace hal;

namespace
{
    // Convert a color to a mapped value using a surface's pixel format.
    Uint32 mapped(const SDL_PixelFormat* fmt, color c)
    {
        return ::SDL_MapRGBA(fmt, c.r, c.g, c.b, c.a);
    }

    SDL_Surface* copy_from(ref<const surface> s)
    {
        const SDL_Surface&     ref { *s->get() };
        const SDL_PixelFormat& fmt { *ref.format };
        return ::SDL_CreateRGBSurfaceWithFormatFrom(ref.pixels, ref.w, ref.h, fmt.BitsPerPixel, ref.pitch, fmt.format);
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
    return ::SDL_FillRect(get(), nullptr, mapped(get()->format, clr));
}

outcome surface::fill(pixel::rect area, color clr)
{
    return ::SDL_FillRect(get(), area.sdl_ptr(), mapped(get()->format, clr));
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

bool surface::must_lock() const
{
    return SDL_MUSTLOCK(get());
}

pixel::const_reference surface::operator[](pixel::point pos) const
{
    HAL_ASSERT(pos.x < get()->w, "Out-of-range width");
    HAL_ASSERT(pos.y < get()->h, "Out-of-range height");

    return { static_cast<std::byte*>(get()->pixels), get()->pitch, get()->format, pos, pass_key<surface> {} };
}

pixel::reference surface::operator[](pixel::point pos)
{
    return { static_cast<std::byte*>(get()->pixels), get()->pitch, get()->format, pos, pass_key<surface> {} };
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

// Blitter.

outcome blitter::operator()()
{
    return ::SDL_BlitScaled(
        m_this.get(),
        m_src.pos.x == unset_pos<src_t>() ? nullptr : m_src.sdl_ptr(),
        m_pass.get(),
        m_dst.pos.x == unset_pos<dst_t>() ? nullptr : m_dst.sdl_ptr());
}

outcome blitter::operator()(HAL_TAG_NAME(keep_dst)) const
{
    pixel::rect copy { m_dst };

    return ::SDL_BlitScaled(
        m_this.get(),
        m_src.pos.x == unset_pos<src_t>() ? nullptr : m_src.sdl_ptr(),
        m_pass.get(),
        m_dst.pos.x == unset_pos<dst_t>() ? nullptr : copy.sdl_ptr());
}
