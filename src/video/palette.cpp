#include <halcyon/video/palette.hpp>

using namespace hal;

palette::palette(int n_colors)
    : resource { ::SDL_CreatePalette(n_colors) }
{
}

bool palette::colors(std::span<const color> c)
{
    return ::SDL_SetPaletteColors(get(), reinterpret_cast<const SDL_Color*>(c.data()), 0, c.size());
}
