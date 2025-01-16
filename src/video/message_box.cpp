#include <halcyon/video/message_box.hpp>

#include <utility>

#include <halcyon/debug.hpp>

using namespace hal;

using msbb = message_box::builder;

namespace
{
    SDL_MessageBoxColor convert(color c)
    {
        return { c.r, c.g, c.b };
    }
}

outcome message_box::show(type tp, const char* title, const char* body)
{
    return ::SDL_ShowSimpleMessageBox(std::to_underlying(tp), title, body, nullptr);
}

msbb::builder()
    : m_btn { { .flags = 0, .buttonid = 0, .text = "Ok" } }
    , m_data {
        .flags       = SDL_MESSAGEBOX_INFORMATION,
        .window      = nullptr,
        .title       = "Halcyon Message Box",
        .message     = "No message provided.",
        .numbuttons  = 1,
        .buttons     = nullptr,
        .colorScheme = nullptr
    }
{
}

msbb& msbb::title(const char* text)
{
    m_data.title = text;

    return *this;
}

msbb& msbb::body(const char* text)
{
    m_data.message = text;

    return *this;
}

msbb& msbb::type(message_box::type tp)
{
    m_data.flags = std::to_underlying(tp);

    return *this;
}

msbb& msbb::buttons(std::initializer_list<const char*> names)
{
    namespace mb = message_box;

    m_data.numbuttons = static_cast<int>(names.size());

    m_btn = decltype(m_btn) { names.size() };

    for (mb::button_t i { 0 }; i < m_data.numbuttons; ++i)
    {
        auto btn_it = m_btn.begin() + i;
        auto str_it = names.begin() + i;

        btn_it->buttonid = i;
        btn_it->flags    = 0;
        btn_it->text     = *str_it;
    }

    return *this;
}

msbb& msbb::colors(color bg, color text, color btn_border, color btn_bg, color btn_select)
{
    m_col = {
        convert(bg), convert(text), convert(btn_border), convert(btn_bg), convert(btn_select)
    };

    m_data.colorScheme = &m_col;

    return *this;
}

msbb& msbb::enter(message_box::button_t idx)
{
    HAL_ASSERT(idx < m_data.numbuttons, "Out of range button ID");

    m_btn[idx].flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;

    return *this;
}

msbb& msbb::escape(message_box::button_t idx)
{
    HAL_ASSERT(idx < m_data.numbuttons, "Out of range button ID");

    m_btn[idx].flags |= SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;

    return *this;
}

msbb& msbb::parent(window& wnd)
{
    m_data.window = wnd.get();

    return *this;
}

message_box::button_t msbb::operator()()
{
    m_data.buttons = m_btn.data();

    int ret;

    if (::SDL_ShowMessageBox(&m_data, &ret) == 0)
        return static_cast<button_t>(ret);
    else
        return -1;
}
