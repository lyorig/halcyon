#include <halcyon/internal/pixel_reference.hpp>

#include <halcyon/system.hpp>

using namespace hal::pixel;

const_reference::const_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos, pass_key<surface>)
    : const_reference { pixels, pitch, fmt, pos }
{
}

const_reference::const_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos)
    : m_ptr { pixels + pos.y * pitch + pos.x * fmt->BytesPerPixel }
    , m_fmt { fmt }
{
}

hal::color const_reference::color() const
{
    hal::color c;

    ::SDL_GetRGBA(get_mapped(), m_fmt, &c.r, &c.g, &c.b, &c.a);

    return c;
}

Uint32 const_reference::get_mapped() const
{
    Uint32 ret { 0 };

    if constexpr (compile_settings::endianness == endian::lil)
    {
        std::memcpy(&ret, m_ptr, m_fmt->BytesPerPixel);
    }

    else
    {
        const std::uint8_t offset { static_cast<std::uint8_t>(sizeof(Uint32) - m_fmt->BytesPerPixel) };
        std::memcpy(&ret + offset, m_ptr + offset, m_fmt->BytesPerPixel);
    }

    return ret;
}

reference::reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos, pass_key<surface>)
    : const_reference { pixels, pitch, fmt, pos }
{
}

void reference::color(struct color c)
{
    set_mapped(::SDL_MapRGBA(m_fmt, c.r, c.g, c.b, c.a));
}

void reference::set_mapped(Uint32 mapped)
{
    if constexpr (compile_settings::endianness == endian::lil)
    {
        std::memcpy(m_ptr, &mapped, m_fmt->BytesPerPixel);
    }

    else
    {
        const std::uint8_t offset = sizeof(Uint32) - m_fmt->BytesPerPixel;
        std::memcpy(m_ptr + offset, &mapped + offset, m_fmt->BytesPerPixel);
    }
}
