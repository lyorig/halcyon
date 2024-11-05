#include <halcyon/video/renderer.hpp>

#include <halcyon/surface.hpp>

#include <halcyon/video/texture.hpp>
#include <halcyon/video/window.hpp>

#include <halcyon/utility/guard.hpp>

using namespace hal;

renderer::renderer(lref<const class window> wnd, flag_bitmask f)
    : resource { ::SDL_CreateRenderer(wnd.get(), -1, f.mask()) }
{
    clear();
}

outcome renderer::clear()
{
    return ::SDL_RenderClear(get());
}

outcome renderer::present()
{
    ::SDL_RenderPresent(get());
    return this->clear();
}

outcome renderer::draw(coord::point pt)
{
    return ::SDL_RenderDrawPointF(get(), pt.x, pt.y);
}

outcome renderer::draw(coord::point pt, struct color c)
{
    guard::color _ { *this, c };
    return draw(pt);
}

outcome renderer::draw(coord::point from, coord::point to)
{
    return ::SDL_RenderDrawLineF(get(), from.x, from.y, to.x, to.y);
}

outcome renderer::draw(coord::point from, coord::point to, struct color c)
{
    guard::color _ { *this, c };
    return draw(from, to);
}

outcome renderer::draw(coord::rect area)
{
    return ::SDL_RenderDrawRectF(get(), area.addr());
}

outcome renderer::draw(coord::rect area, struct color c)
{
    guard::color _ { *this, c };
    return draw(area);
}

copyer renderer::draw(ref<const texture> tx)
{
    return { *this, tx };
}

outcome renderer::fill(coord::rect area)
{
    return ::SDL_RenderFillRectF(get(), area.addr());
}

outcome renderer::fill(coord::rect area, struct color c)
{
    guard::color _ { *this, c };
    return fill(area);
}

outcome renderer::fill(std::span<const coord::rect> areas)
{
    return ::SDL_RenderFillRectsF(get(), areas.front().addr(), static_cast<int>(areas.size()));
}

outcome renderer::fill(std::span<const coord::rect> areas, struct color c)
{
    guard::color _ { *this, c };
    return fill(areas);
}

outcome renderer::fill()
{
    return ::SDL_RenderFillRect(get(), nullptr);
}

outcome renderer::fill(struct color c)
{
    guard::color _ { *this, c };
    return fill();
}

outcome renderer::target(ref<target_texture> tx)
{
    return internal_target(tx.get());
}

outcome renderer::reset_target()
{
    return this->internal_target(nullptr);
}

surface renderer::read_pixels() const
{
    pixel::format fmt { window()->pixel_format() };
    hal::surface  surf { size().get(), fmt };

    if (!outcome { ::SDL_RenderReadPixels(get(), nullptr, static_cast<Uint32>(fmt), surf.get()->pixels, surf.get()->pitch) })
        surf.reset();

    return surf;
}

surface renderer::read_pixels(pixel::rect area) const
{
    pixel::format fmt { window()->pixel_format() };
    hal::surface  surf { area.size, fmt };

    if (!outcome { ::SDL_RenderReadPixels(get(), area.addr(), static_cast<Uint32>(fmt), surf.get()->pixels, surf.get()->pitch) })
        surf.reset();

    return surf;
}

result<color> renderer::color() const
{
    hal::color ret;

    return { ::SDL_GetRenderDrawColor(get(), &ret.r, &ret.g, &ret.b, &ret.a), ret };
}

outcome renderer::color(hal::color clr)
{
    return ::SDL_SetRenderDrawColor(get(), clr.r, clr.g, clr.b, clr.a);
}

result<blend_mode> renderer::blend() const
{
    SDL_BlendMode bm;

    return { ::SDL_GetRenderDrawBlendMode(get(), &bm), static_cast<blend_mode>(bm) };
}

outcome renderer::blend(blend_mode bm)
{
    return ::SDL_SetRenderDrawBlendMode(get(), SDL_BlendMode(bm));
}

result<pixel::point> renderer::size() const
{
    pixel::point sz;
    ::SDL_RenderGetLogicalSize(get(), &sz.x, &sz.y);

    if (sz.x == 0)
        return { ::SDL_GetRendererOutputSize(get(), &sz.x, &sz.y), sz };
    else
        return { outcome::success, sz };
}

outcome renderer::size(pixel::point sz)
{
    return ::SDL_RenderSetLogicalSize(get(), sz.x, sz.y);
}

ref<const window> renderer::window() const
{
    return { ::SDL_RenderGetWindow(get()), pass_key<renderer> {} };
}

ref<window> renderer::window()
{
    return { ::SDL_RenderGetWindow(get()), pass_key<renderer> {} };
}

result<renderer_info> renderer::info() const
{
    renderer_info ret;

    return { ::SDL_GetRendererInfo(get(), &ret), ret };
}

outcome renderer::internal_target(SDL_Texture* target)
{
    return ::SDL_SetRenderTarget(get(), target);
}

c_string renderer_info::name() const
{
    return SDL_RendererInfo::name;
}

renderer::flag_bitmask renderer_info::flags() const
{
    return static_cast<renderer::flag>(SDL_RendererInfo::flags);
}

std::span<const pixel::format> renderer_info::formats() const
{
    static_assert(sizeof(pixel::format) == sizeof(texture_formats[0]));

    return {
        reinterpret_cast<const pixel::format*>(texture_formats),
        static_cast<std::size_t>(num_texture_formats)
    };
}

pixel::point renderer_info::max_texture_size() const
{
    return { max_texture_width, max_texture_height };
}

std::ostream& hal::operator<<(std::ostream& str, const renderer_info& info)
{
    return str << '[' << info.name() << ", max tex. size " << info.max_texture_size() << ", flags " << info.flags().mask() << ']';
}

// Copyer.

copyer& copyer::rotate(f64 angle)
{
    m_angle = angle;
    return *this;
}

copyer& copyer::flip(enum flip f)
{
    m_flip = f;
    return *this;
}

copyer& copyer::outline()
{
    m_pass->draw(m_dst);
    return *this;
}

copyer& copyer::outline(color c)
{
    guard::color _ { m_pass, c };
    return outline();
}

outcome copyer::operator()()
{
    return ::SDL_RenderCopyExF(m_pass.get(), m_this.get(),
        m_src.pos.x == unset_pos<src_t>() ? nullptr : m_src.addr(),
        m_dst.pos.x == unset_pos<dst_t>() ? nullptr : m_dst.addr(),
        m_angle, nullptr, static_cast<SDL_RendererFlip>(m_flip));
}
