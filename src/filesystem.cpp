#include <halcyon/filesystem.hpp>

#include "SDL3/SDL_filesystem.h"

using namespace hal;

c_string hal::base_path()
{
    return ::SDL_GetBasePath();
}

// SDL caches this string so it's okay
// to use it with a std::string_view.
resource_loader::resource_loader()
    : m_base { base_path() }
{
}

std::string resource_loader::resolve(std::string_view path) const
{
    std::string       ret;
    const std::size_t new_size { m_base.size() + path.size() };

    ret.resize_and_overwrite(new_size,
        [this, path, new_size](char* buf, std::size_t)
        {
            std::memcpy(buf, m_base.data(), m_base.size());
            std::memcpy(buf + m_base.size(), path.data(), path.size());

            return new_size;
        });

    return ret;
}

accessor resource_loader::access(std::string_view path) const
{
    return resolve(path);
}

outputter resource_loader::output(std::string_view path) const
{
    return resolve(path);
}

std::string_view resource_loader::base() const
{
    return m_base;
}
