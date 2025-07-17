#include <halcyon/video.hpp>

#include <halcyon/utility/strutil.hpp>

using namespace hal;

// ---- WINDOW CREATE PROPERTIES ----

// FBI, OPEN UP!
using cp = window::create_properties;

#define PROP_BFUNC(func_name, prop_name)                             \
    cp& cp::func_name(bool val)                                      \
    {                                                                \
        bool_set(SDL_PROP_WINDOW_CREATE_##prop_name##_BOOLEAN, val); \
        return *this;                                                \
    }

PROP_BFUNC(always_on_top, ALWAYS_ON_TOP);
PROP_BFUNC(external_graphics_context, EXTERNAL_GRAPHICS_CONTEXT);
PROP_BFUNC(focusable, FOCUSABLE);
PROP_BFUNC(resizable, RESIZABLE);
PROP_BFUNC(hidden, HIDDEN);
PROP_BFUNC(high_pixel_density, HIGH_PIXEL_DENSITY);
PROP_BFUNC(maximized, MAXIMIZED);
PROP_BFUNC(minimized, MINIMIZED);
PROP_BFUNC(fullscreen, FULLSCREEN);
PROP_BFUNC(borderless, BORDERLESS);
PROP_BFUNC(transparent, TRANSPARENT);
PROP_BFUNC(menu, MENU);
PROP_BFUNC(modal, MODAL);
PROP_BFUNC(tooltip, TOOLTIP);
PROP_BFUNC(utility, UTILITY);
PROP_BFUNC(constrain_popup, CONSTRAIN_POPUP);
PROP_BFUNC(mouse_grabbed, MOUSE_GRABBED);
PROP_BFUNC(metal, METAL);
PROP_BFUNC(opengl, OPENGL);
PROP_BFUNC(vulkan, VULKAN);

cp& cp::parent(ref<window> val)
{
    ptr_set(SDL_PROP_WINDOW_CREATE_PARENT_POINTER, val.get());
    return *this;
}

cp& cp::title(c_string val)
{
    string_set(SDL_PROP_WINDOW_CREATE_TITLE_STRING, val.c_str());
    return *this;
}

cp& cp::size(pixel::point val)
{
    number_set(SDL_PROP_WINDOW_CREATE_WIDTH_NUMBER, val.x);
    number_set(SDL_PROP_WINDOW_CREATE_HEIGHT_NUMBER, val.y);

    return *this;
}

cp& cp::pos(pixel::point val)
{
    number_set(SDL_PROP_WINDOW_CREATE_X_NUMBER, val.x);
    number_set(SDL_PROP_WINDOW_CREATE_Y_NUMBER, val.y);

    return *this;
}

// ---- WINDOW PROPERTIES ----

using wp = window::properties;

wp::properties(id_t id, pass_key<window>)
    : properties_ref { id }
{
}

ref<surface> wp::shape() const
{
    return ref<hal::surface>::from_ptr(
        reinterpret_cast<SDL_Surface*>(
            ptr_get(SDL_PROP_WINDOW_SHAPE_POINTER, nullptr)));
}

bool wp::hdr() const
{
    return bool_get(SDL_PROP_WINDOW_HDR_ENABLED_BOOLEAN, false);
}

float wp::hdr_headroom() const
{
    return float_get(SDL_PROP_WINDOW_HDR_HEADROOM_FLOAT, 0.f);
}

float wp::sdr_white_level() const
{
    return float_get(SDL_PROP_WINDOW_SDR_WHITE_LEVEL_FLOAT, 0.f);
}

// ---- WINDOW ----

window::window(proxy::video, c_string title, pixel::point size, flag_bitmask f)
    : resource { ::SDL_CreateWindow(title.data(), size.x, size.y, f.mask()) }
{
}

window::window(proxy::video, const create_properties& props)
    : resource { ::SDL_CreateWindowWithProperties(props.id()) }
{
}

display::id_t window::display_index() const
{
    return ::SDL_GetDisplayForWindow(get());
}

pixel::format window::pixel_format() const
{
    return static_cast<pixel::format>(::SDL_GetWindowPixelFormat(get()));
}

window::id_t window::id() const
{
    return static_cast<window::id_t>(::SDL_GetWindowID(get()));
}

window::flag_bitmask window::flags() const
{
    return static_cast<enum flag>(::SDL_GetWindowFlags(get()));
}

hal::pixel::point window::pos() const
{
    point<int> ret;

    ::SDL_GetWindowPosition(get(), &ret.x, &ret.y);

    return ret;
}

void window::pos(hal::pixel::point ps)
{
    ::SDL_SetWindowPosition(get(), ps.x, ps.y);
}

hal::pixel::point window::size() const
{
    point<int> size;

    ::SDL_GetWindowSize(get(), &size.x, &size.y);

    return size;
}

void window::size(pixel::point sz)
{
    HAL_WARN_IF(fullscreen(), "Setting size of fullscreen window - this does nothing");

    ::SDL_SetWindowSize(get(), sz.x, sz.y);
}

pixel::point window::min_size() const
{
    pixel::point ret;

    ::SDL_GetWindowMinimumSize(get(), &ret.x, &ret.y);

    return ret;
}

void window::min_size(pixel::point pt)
{
    ::SDL_SetWindowMinimumSize(get(), pt.x, pt.y);
}

pixel::point window::max_size() const
{
    pixel::point ret;

    ::SDL_GetWindowMaximumSize(get(), &ret.x, &ret.y);

    return ret;
}

void window::max_size(pixel::point pt)
{
    ::SDL_SetWindowMaximumSize(get(), pt.x, pt.y);
}

c_string window::title() const
{
    return ::SDL_GetWindowTitle(get());
}

void window::title(const char* val)
{
    ::SDL_SetWindowTitle(get(), val);
}

bool window::fullscreen() const
{
    return flags().any(flag::fullscreen);
}

void window::always_on_top(bool set)
{
    ::SDL_SetWindowAlwaysOnTop(get(), set);
}

bool window::fullscreen(bool set)
{
    return ::SDL_SetWindowFullscreen(get(), set * std::to_underlying(window::flag::fullscreen));
}

ref<const renderer> window::renderer() const
{
    return ref<const hal::renderer>::from_ptr(::SDL_GetRenderer(get()));
}

ref<renderer> window::renderer()
{
    return ref<hal::renderer>::from_ptr(::SDL_GetRenderer(get()));
}

ref<const surface> window::surface() const
{
    return ref<const hal::surface>::from_ptr(::SDL_GetWindowSurface(get()));
}

window::properties window::props() const
{
    return { ::SDL_GetWindowProperties(get()), pass_key<window> {} };
}

std::ostream& hal::operator<<(std::ostream& str, hal::ref<hal::window> wnd)
{
    if (!wnd->valid())
    {
        str << "[invalid]";
        return str;
    }

    str << "[id: " << to_printable_int(wnd->id())
        << ", title: " << wnd->title()
        << ", size: " << wnd->size()
        << ", pos: " << wnd->pos()
        << ", display: " << to_printable_int(wnd->display_index())
        << ", pixel format: " << wnd->pixel_format()
        << ", flags: " << int_to_hex_array(wnd->flags().mask()).data()
        << ']';

    return str;
}
