#include <SDL_render.h>
#include <halcyon/video/renderer.hpp>

#include <halcyon/surface.hpp>

#include <halcyon/video/texture.hpp>
#include <halcyon/video/window.hpp>

#include <halcyon/utility/locks.hpp>

using namespace hal;

renderer::renderer(clref<class window> wnd, flag_bitmask f)
    : raii_object { ::SDL_CreateRenderer(wnd->get(), -1, f.mask()) }
{
    HAL_PRINT("Created renderer for \"", wnd->title(), "\" ");
}

void renderer::clear()
{
    HAL_ASSERT_VITAL(::SDL_RenderClear(get()) == 0, debug::last_error());
}

void renderer::present()
{
    ::SDL_RenderPresent(get());
    this->clear();
}

void renderer::draw(coord::point pt)
{
    ::SDL_RenderDrawPointF(get(), pt.x, pt.y);
}

void renderer::draw(coord::point from, coord::point to)
{
    HAL_ASSERT_VITAL(::SDL_RenderDrawLineF(get(), from.x, from.y, to.x, to.y) == 0, debug::last_error());
}

void renderer::draw(coord::rect area)
{
    HAL_ASSERT_VITAL(::SDL_RenderDrawRectF(get(), area.addr()) == 0, debug::last_error());
}

void renderer::fill(coord::rect area)
{
    HAL_ASSERT_VITAL(::SDL_RenderFillRectF(get(), area.addr()) == 0, debug::last_error());
}

void renderer::fill(std::span<const coord::rect> areas)
{
    HAL_ASSERT_VITAL(::SDL_RenderFillRectsF(get(), areas.front().addr(), static_cast<int>(areas.size())) == 0, debug::last_error());
}

void renderer::fill()
{
    HAL_ASSERT_VITAL(::SDL_RenderFillRect(get(), nullptr) == 0, debug::last_error());
}

void renderer::target(ref<target_texture> tx)
{
    this->internal_target(tx->get());
}

void renderer::reset_target()
{
    this->internal_target(nullptr);
}

surface renderer::read_pixels(pixel::format fmt)
{
    hal::surface surf { size(), fmt };

    HAL_ASSERT_VITAL(::SDL_RenderReadPixels(get(), nullptr, static_cast<Uint32>(fmt), surf.get()->pixels, surf.get()->pitch) == 0, debug::last_error());

    return surf;
}

surface renderer::read_pixels(pixel::rect area, pixel::format fmt)
{
    hal::surface surf { area.size, fmt };

    HAL_ASSERT_VITAL(::SDL_RenderReadPixels(get(), area.addr(), static_cast<Uint32>(fmt), surf.get()->pixels, surf.get()->pitch) == 0, debug::last_error());

    return surf;
}

color renderer::color() const
{
    hal::color ret;

    HAL_ASSERT_VITAL(::SDL_GetRenderDrawColor(get(), &ret.r, &ret.g, &ret.b, &ret.a) == 0, debug::last_error());

    return ret;
}

void renderer::color(hal::color clr)
{
    HAL_ASSERT_VITAL(::SDL_SetRenderDrawColor(get(), clr.r, clr.g, clr.b, clr.a) == 0, debug::last_error());
}

blend_mode renderer::blend() const
{
    SDL_BlendMode bm;

    HAL_ASSERT_VITAL(::SDL_GetRenderDrawBlendMode(get(), &bm) == 0, debug::last_error());

    return static_cast<blend_mode>(bm);
}

void renderer::blend(blend_mode bm)
{
    HAL_ASSERT_VITAL(::SDL_SetRenderDrawBlendMode(get(), SDL_BlendMode(bm)) == 0, debug::last_error());
}

pixel::point renderer::size() const
{
    hal::point<int> sz;
    ::SDL_RenderGetLogicalSize(get(), &sz.x, &sz.y);

    if (sz.x == 0)
        HAL_ASSERT_VITAL(::SDL_GetRendererOutputSize(get(), &sz.x, &sz.y) == 0, debug::last_error());

    return static_cast<pixel::point>(sz);
}

void renderer::size(pixel::point sz)
{
    HAL_ASSERT_VITAL(::SDL_RenderSetLogicalSize(get(), sz.x, sz.y) == 0, debug::last_error());
}

void renderer::size(scaler scl)
{
    size(scl(size()));
}

ref<const window> renderer::window() const
{
    return { ::SDL_RenderGetWindow(get()), pass_key<renderer> {} };
}

ref<window> renderer::window()
{
    return { ::SDL_RenderGetWindow(get()), pass_key<renderer> {} };
}

info::sdl::renderer renderer::info() const
{
    return { *this, pass_key<renderer> {} };
}

static_texture renderer::make_static_texture(ref<const surface> surf) const&
{
    return { *this, surf };
}

target_texture renderer::make_target_texture(pixel::point size, pixel::format fmt) const&
{
    return { *this, size, fmt };
}

streaming_texture renderer::make_streaming_texture(pixel::point size, pixel::format fmt) const&
{
    return { *this, size, fmt };
}

copyer renderer::render(ref<const texture> tex)
{
    return { *this, tex };
}

void renderer::internal_target(SDL_Texture* target)
{
    HAL_ASSERT_VITAL(::SDL_SetRenderTarget(get(), target) == 0, debug::last_error());
}

// Renderer information (SDL).

info::sdl::renderer::renderer(ref<const hal::renderer> rnd, pass_key<hal::renderer>)
{
    HAL_ASSERT_VITAL(::SDL_GetRendererInfo(rnd->get(), static_cast<SDL_RendererInfo*>(this)) == 0, debug::last_error());
}

std::string_view info::sdl::renderer::name() const
{
    return SDL_RendererInfo::name;
}

renderer::flag_bitmask info::sdl::renderer::flags() const
{
    return static_cast<hal::renderer::flag>(SDL_RendererInfo::flags);
}

std::span<const pixel::format> info::sdl::renderer::formats() const
{
    static_assert(sizeof(pixel::format) == sizeof(Uint32));
    return { reinterpret_cast<const pixel::format*>(texture_formats), static_cast<std::size_t>(num_texture_formats) };
}

pixel::point info::sdl::renderer::max_texture_size() const
{
    return {
        static_cast<pixel_t>(max_texture_width),
        static_cast<pixel_t>(max_texture_height)
    };
}

SDL_RendererInfo* info::sdl::renderer::get()
{
    return this;
}

std::ostream& info::sdl::operator<<(std::ostream& str, const info::sdl::renderer& inf)
{
    str << "[name: " << inf.name() << ", flags: 0x" << std::hex << inf.flags().mask() << std::dec << ", formats: [";

    const auto span = inf.formats();

    for (auto iter = span.begin(); iter != span.end(); ++iter)
    {
        if (iter != span.begin())
            str << ", ";

        str << hal::to_string(*iter);
    }

    return str << "], MTS: " << inf.max_texture_size() << "]]";
}

// Renderer information (compressed).

info::renderer::renderer(const sdl::renderer& src)
    : name { src.name() }
    , max_texture_size { src.max_texture_size() }
    , flags { src.flags() }
    , formats { src.formats() }

{
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
    lock::color<renderer> _ { m_pass, c };
    return outline();
}

void copyer::operator()()
{
    HAL_ASSERT_VITAL(::SDL_RenderCopyExF(m_pass->get(), m_this->get(),
                         m_src.pos.x == unset_pos<src_t>() ? nullptr : m_src.addr(),
                         m_dst.pos.x == unset_pos<dst_t>() ? nullptr : m_dst.addr(),
                         m_angle, nullptr, static_cast<SDL_RendererFlip>(m_flip))
            == 0,
        debug::last_error());
}
