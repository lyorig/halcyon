#include <SDL3/SDL_render.h>
#include <halcyon/video/renderer.hpp>

#include <halcyon/surface.hpp>

#include <halcyon/video/texture.hpp>
#include <halcyon/video/window.hpp>

#include <halcyon/utility/guard.hpp>
#include <halcyon/utility/strutil.hpp>

using namespace hal;

renderer::renderer(lref<const class window> wnd)
    : resource { ::SDL_CreateRenderer(wnd.get(), nullptr) }
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
    return ::SDL_RenderPoint(get(), pt.x, pt.y);
}

outcome renderer::draw(coord::point pt, struct color c)
{
    guard::color _ { *this, c };
    return draw(pt);
}

outcome renderer::draw(std::span<const coord::point> pts)
{
    return ::SDL_RenderPoints(get(), reinterpret_cast<const SDL_FPoint*>(pts.data()), static_cast<int>(pts.size()));
}

outcome renderer::draw(std::span<const coord::point> areas, struct color c)
{
    guard::color _ { *this, c };
    return draw(areas);
}

outcome renderer::draw(coord::point from, coord::point to)
{
    return ::SDL_RenderLine(get(), from.x, from.y, to.x, to.y);
}

outcome renderer::draw(coord::point from, coord::point to, struct color c)
{
    guard::color _ { *this, c };
    return draw(from, to);
}

outcome renderer::draw(std::span<const coord::point> pts, HAL_TAG_NAME(connect))
{
    return ::SDL_RenderLines(get(), reinterpret_cast<const SDL_FPoint*>(pts.data()), static_cast<int>(pts.size()));
}

outcome renderer::draw(std::span<const coord::point> areas, struct color c, HAL_TAG_NAME(connect) tag)
{
    guard::color _ { *this, c };
    return draw(areas, tag);
}

outcome renderer::draw(coord::rect area)
{
    return ::SDL_RenderRect(get(), area.sdl_ptr());
}

outcome renderer::draw(coord::rect area, struct color c)
{
    guard::color _ { *this, c };
    return draw(area);
}

outcome renderer::draw(std::span<const coord::rect> pts)
{
    return ::SDL_RenderRects(get(), reinterpret_cast<const SDL_FRect*>(pts.data()), static_cast<int>(pts.size()));
}

outcome renderer::draw(std::span<const coord::rect> areas, struct color c)
{
    guard::color _ { *this, c };
    return draw(areas);
}

copyer renderer::draw(ref<const texture> tx)
{
    return { *this, tx };
}

outcome renderer::fill(coord::rect area)
{
    return ::SDL_RenderFillRect(get(), area.sdl_ptr());
}

outcome renderer::fill(coord::rect area, struct color c)
{
    guard::color _ { *this, c };
    return fill(area);
}

outcome renderer::fill(std::span<const coord::rect> areas)
{
    return ::SDL_RenderFillRects(get(), reinterpret_cast<const SDL_FRect*>(areas.data()), static_cast<int>(areas.size()));
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
    return ::SDL_RenderReadPixels(get(), nullptr);
}

surface renderer::read_pixels(pixel::rect area) const
{
    return ::SDL_RenderReadPixels(get(), area.sdl_ptr());
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
    pixel::point ret;
    return { ::SDL_GetCurrentRenderOutputSize(get(), &ret.x, &ret.y), ret };
}

outcome renderer::size(pixel::point sz, presentation p)
{
    return ::SDL_SetRenderLogicalPresentation(get(), sz.x, sz.y, static_cast<SDL_RendererLogicalPresentation>(p));
}

ref<const window> renderer::window() const
{
    return { ::SDL_GetRenderWindow(get()), pass_key<renderer> {} };
}

ref<window> renderer::window()
{
    return { ::SDL_GetRenderWindow(get()), pass_key<renderer> {} };
}

outcome renderer::internal_target(SDL_Texture* target)
{
    return ::SDL_SetRenderTarget(get(), target);
}

// Copyer.

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

outcome copyer::render()
{
    return ::SDL_RenderTexture(
        m_pass.get(),
        m_this.get(),
        m_src.pos.x == unset_pos<src_t>() ? nullptr : m_src.sdl_ptr(),
        m_dst.pos.x == unset_pos<dst_t>() ? nullptr : m_dst.sdl_ptr());
}

outcome copyer::rotated(double angle, flip f)
{
    return ::SDL_RenderTextureRotated(
        m_pass.get(),
        m_this.get(),
        m_src.pos.x == unset_pos<src_t>() ? nullptr : m_src.sdl_ptr(),
        m_dst.pos.x == unset_pos<dst_t>() ? nullptr : m_dst.sdl_ptr(),
        angle,
        nullptr,
        static_cast<SDL_FlipMode>(f));
}

outcome copyer::affine(coord::point right, coord::point down)
{
    return ::SDL_RenderTextureAffine(
        m_pass.get(),
        m_this.get(),
        m_src.pos.x == unset_pos<src_t>() ? nullptr : m_src.sdl_ptr(),
        m_dst.pos.x == unset_pos<dst_t>() ? nullptr : m_dst.pos.sdl_ptr(),
        right.sdl_ptr(),
        down.sdl_ptr());
}

outcome copyer::tiled(float scale)
{
    return ::SDL_RenderTextureTiled(
        m_pass.get(),
        m_this.get(),
        m_src.pos.x == unset_pos<src_t>() ? nullptr : m_src.sdl_ptr(),
        scale,
        m_dst.pos.x == unset_pos<dst_t>() ? nullptr : m_dst.sdl_ptr());
}
