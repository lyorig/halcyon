#define _CRT_SECURE_NO_WARNINGS

#include <halcyon/internal/rwops.hpp>

#include <halcyon/utility/buffer.hpp>

using namespace hal;

const char* detail::path_cvt(const char* path)
{
    return path;
}

detail::wchar_cvt::wchar_cvt(const wchar_t* path)
    : m_buf(std::wcslen(path) * sizeof(wchar_t) + 1)
{
    if (std::wcstombs(m_buf.data(), path, m_buf.size()) == -1)
        m_buf = {};
}

detail::wchar_cvt::operator const char*() const
{
    return m_buf.data();
}

detail::wchar_cvt detail::path_cvt(const wchar_t* path)
{
    return path;
}

SDL_RWops* accessor::get(pass_key<image::context>) const
{
    return resource::get();
}

SDL_RWops* accessor::use(pass_key<surface>)
{
    return resource::release();
}

SDL_RWops* accessor::use(pass_key<font>)
{
    return resource::release();
}

SDL_RWops* accessor::use(pass_key<image::context>)
{
    return resource::release();
}

SDL_RWops* outputter::use(pass_key<surface>)
{
    return resource::release();
}

SDL_RWops* outputter::use(pass_key<image::context>)
{
    return resource::release();
}
