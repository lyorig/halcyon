#define _CRT_SECURE_NO_WARNINGS

#include <halcyon/internal/iostream.hpp>

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
