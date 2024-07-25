#define _CRT_SECURE_NO_WARNINGS

#include <halcyon/utility/strutil.hpp>

#include <halcyon/debug.hpp>

using namespace hal;

std::unique_ptr<char[]> hal::wide_to_multibyte(non_null<const wchar_t> str)
{
    const std::size_t len { hal::strlen(str.get()) * sizeof(wchar_t) };

    auto ret { std::make_unique<char[]>(len + 1) };

    HAL_ASSERT_VITAL(std::wcstombs(ret.get(), str.get(), len) != static_cast<std::size_t>(-1), "std::wcstombs() returned error value");

    return ret;
}
