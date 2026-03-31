#include <halcyon/filesystem.hpp>

#include <SDL3/SDL_filesystem.h>

using namespace hal;

const char* fs::base_path()
{
    return ::SDL_GetBasePath();
}

const char* fs::pref_path(const char* org, const char* app)
{
    return ::SDL_GetPrefPath(org, app);
}

// SDL caches this string so it's okay
// to use it with a `std::string_view`.
fs::resource_loader::resource_loader()
    : resource_loader { base_path() }
{
}

fs::resource_loader::resource_loader(std::string_view base)
    : m_base { base }
{
}

std::string fs::resource_loader::resolve(std::string_view path) const
{
    return resolve(m_base, path);
}

std::string fs::resource_loader::resolve(std::string_view base, std::string_view path)
{
    std::string       ret;
    const std::size_t new_size { base.size() + path.size() };

    ret.resize_and_overwrite(new_size,
        [base, path, new_size](char* buf, std::size_t)
        {
            std::memcpy(buf, base.data(), base.size());
            std::memcpy(buf + base.size(), path.data(), path.size());

            return new_size;
        });

    return ret;
}

accessor fs::resource_loader::access(std::string_view path) const
{
    return resolve(path);
}

outputter fs::resource_loader::output(std::string_view path) const
{
    return resolve(path);
}

std::string_view fs::resource_loader::base() const
{
    return m_base;
}
