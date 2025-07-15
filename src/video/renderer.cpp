#include <SDL3/SDL_render.h>
#include <halcyon/video/renderer.hpp>

#include <halcyon/surface.hpp>

#include <halcyon/video/texture.hpp>
#include <halcyon/video/window.hpp>

#include <halcyon/utility/guard.hpp>
#include <halcyon/utility/strutil.hpp>

using namespace hal;

// ----- CREATE PROPERTIES -----

// FBI, OPEN UP!
using cp = renderer::create_properties;

cp& cp::name(c_string val)
{
    string_set(SDL_PROP_RENDERER_CREATE_NAME_STRING, val.c_str());
    return *this;
}

cp& cp::window(ref<hal::window> val)
{
    ptr_set(SDL_PROP_RENDERER_CREATE_WINDOW_POINTER, val.get());
    return *this;
}

cp& cp::surface(ref<hal::surface> val)
{
    ptr_set(SDL_PROP_RENDERER_CREATE_SURFACE_POINTER, val.get());
    return *this;
}

cp& cp::vsync(bool val)
{
    number_set(SDL_PROP_RENDERER_CREATE_PRESENT_VSYNC_NUMBER, val);
    return *this;
}

// ----- PROPERTIES -----

using rp = renderer::properties;

rp::properties(SDL_PropertiesID id, pass_key<renderer>)
    : properties_ref { id }
{
}

c_string rp::name() const
{
    return string_get(SDL_PROP_RENDERER_NAME_STRING, {});
}

ref<window> rp::window()
{
    return ref<hal::window>::from_ptr(
        reinterpret_cast<SDL_Window*>(
            ptr_get(SDL_PROP_RENDERER_WINDOW_POINTER, nullptr)));
}

ref<const window> rp::window() const
{
    return ref<const hal::window>::from_ptr(
        reinterpret_cast<SDL_Window*>(
            ptr_get(SDL_PROP_RENDERER_WINDOW_POINTER, nullptr)));
}

bool rp::vsync() const
{
    return static_cast<bool>(number_get(SDL_PROP_RENDERER_VSYNC_NUMBER, 0));
}

std::int64_t rp::max_texture_size() const
{
    return number_get(SDL_PROP_RENDERER_MAX_TEXTURE_SIZE_NUMBER, 0);
}

const pixel::format* rp::formats() const
{
    return reinterpret_cast<const pixel::format*>(
        ptr_get(SDL_PROP_RENDERER_TEXTURE_FORMATS_POINTER, nullptr));
}

// ----- RENDERER -----

renderer::renderer(lref<const hal::window> wnd)
    : resource { ::SDL_CreateRenderer(wnd.get(), nullptr) }
{
}

renderer::renderer(const create_properties& props)
    : resource { ::SDL_CreateRendererWithProperties(props.id()) }
{
}

bool renderer::clear()
{
    return ::SDL_RenderClear(get());
}

bool renderer::present()
{
    ::SDL_RenderPresent(get());
    return this->clear();
}

bool renderer::draw(coord::point pt)
{
    return ::SDL_RenderPoint(get(), pt.x, pt.y);
}

bool renderer::draw(coord::point pt, struct color c)
{
    guard::color _ { *this, c };
    return draw(pt);
}

bool renderer::draw(std::span<const coord::point> pts)
{
    return ::SDL_RenderPoints(get(), reinterpret_cast<const SDL_FPoint*>(pts.data()), static_cast<int>(pts.size()));
}

bool renderer::draw(std::span<const coord::point> areas, struct color c)
{
    guard::color _ { *this, c };
    return draw(areas);
}

bool renderer::draw(coord::point from, coord::point to)
{
    return ::SDL_RenderLine(get(), from.x, from.y, to.x, to.y);
}

bool renderer::draw(coord::point from, coord::point to, struct color c)
{
    guard::color _ { *this, c };
    return draw(from, to);
}

bool renderer::draw(std::span<const coord::point> pts, HAL_TAG_NAME(connect))
{
    return ::SDL_RenderLines(get(), reinterpret_cast<const SDL_FPoint*>(pts.data()), static_cast<int>(pts.size()));
}

bool renderer::draw(std::span<const coord::point> areas, struct color c, HAL_TAG_NAME(connect) tag)
{
    guard::color _ { *this, c };
    return draw(areas, tag);
}

bool renderer::draw(coord::rect area)
{
    return ::SDL_RenderRect(get(), area.sdl_ptr());
}

bool renderer::draw(coord::rect area, struct color c)
{
    guard::color _ { *this, c };
    return draw(area);
}

bool renderer::draw(std::span<const coord::rect> pts)
{
    return ::SDL_RenderRects(get(), reinterpret_cast<const SDL_FRect*>(pts.data()), static_cast<int>(pts.size()));
}

bool renderer::draw(std::span<const coord::rect> areas, struct color c)
{
    guard::color _ { *this, c };
    return draw(areas);
}

copyer renderer::draw(ref<const texture> tx)
{
    return { *this, tx };
}

bool renderer::fill(coord::rect area)
{
    return ::SDL_RenderFillRect(get(), area.sdl_ptr());
}

bool renderer::fill(coord::rect area, struct color c)
{
    guard::color _ { *this, c };
    return fill(area);
}

bool renderer::fill(std::span<const coord::rect> areas)
{
    return ::SDL_RenderFillRects(get(), reinterpret_cast<const SDL_FRect*>(areas.data()), static_cast<int>(areas.size()));
}

bool renderer::fill(std::span<const coord::rect> areas, struct color c)
{
    guard::color _ { *this, c };
    return fill(areas);
}

bool renderer::fill()
{
    return ::SDL_RenderFillRect(get(), nullptr);
}

bool renderer::fill(struct color c)
{
    guard::color _ { *this, c };
    return fill();
}

bool renderer::target(ref<target_texture> tx)
{
    return internal_target(tx.get());
}

bool renderer::reset_target()
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

bool renderer::color(hal::color clr)
{
    return ::SDL_SetRenderDrawColor(get(), clr.r, clr.g, clr.b, clr.a);
}

result<blend_mode> renderer::blend() const
{
    SDL_BlendMode bm;

    return { ::SDL_GetRenderDrawBlendMode(get(), &bm), static_cast<blend_mode>(bm) };
}

bool renderer::blend(blend_mode bm)
{
    return ::SDL_SetRenderDrawBlendMode(get(), SDL_BlendMode(bm));
}

result<pixel::point> renderer::size() const
{
    pixel::point ret;
    return { ::SDL_GetCurrentRenderOutputSize(get(), &ret.x, &ret.y), ret };
}

bool renderer::size(pixel::point sz, presentation p)
{
    return ::SDL_SetRenderLogicalPresentation(get(), sz.x, sz.y, static_cast<SDL_RendererLogicalPresentation>(p));
}

ref<const window> renderer::window() const
{
    return ref<const hal::window>::from_ptr(
        ::SDL_GetRenderWindow(get()));
}

ref<window> renderer::window()
{
    return ref<hal::window>::from_ptr(
        ::SDL_GetRenderWindow(get()));
}

const char* renderer::name() const
{
    return ::SDL_GetRendererName(get());
}

rp renderer::props() const
{
    return { ::SDL_GetRendererProperties(get()), pass_key<renderer> {} };
}

bool renderer::internal_target(SDL_Texture* target)
{
    return ::SDL_SetRenderTarget(get(), target);
}

// Copyer.

copyer& copyer::outline()
{
    m_drawDst->draw(m_posDst);
    return *this;
}

copyer& copyer::outline(color c)
{
    guard::color _ { m_drawDst, c };
    return outline();
}

bool copyer::render()
{
    return ::SDL_RenderTexture(
        m_drawDst.get(),
        m_drawSrc.get(),
        m_posSrc.pos.x == unset_pos() ? nullptr : m_posSrc.sdl_ptr(),
        m_posDst.pos.x == unset_pos() ? nullptr : m_posDst.sdl_ptr());
}

bool copyer::rotated(double angle, flip f)
{
    return ::SDL_RenderTextureRotated(
        m_drawDst.get(),
        m_drawSrc.get(),
        m_posSrc.pos.x == unset_pos() ? nullptr : m_posSrc.sdl_ptr(),
        m_posDst.pos.x == unset_pos() ? nullptr : m_posDst.sdl_ptr(),
        angle,
        nullptr,
        static_cast<SDL_FlipMode>(f));
}

bool copyer::affine(coord::point right, coord::point down)
{
    return ::SDL_RenderTextureAffine(
        m_drawDst.get(),
        m_drawSrc.get(),
        m_posSrc.pos.x == unset_pos() ? nullptr : m_posSrc.sdl_ptr(),
        m_posDst.pos.x == unset_pos() ? nullptr : m_posDst.pos.sdl_ptr(),
        right.sdl_ptr(),
        down.sdl_ptr());
}

bool copyer::tiled(float scale)
{
    return ::SDL_RenderTextureTiled(
        m_drawDst.get(),
        m_drawSrc.get(),
        m_posSrc.pos.x == unset_pos() ? nullptr : m_posSrc.sdl_ptr(),
        scale,
        m_posDst.pos.x == unset_pos() ? nullptr : m_posDst.sdl_ptr());
}
