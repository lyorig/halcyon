#include <halcyon/utility/guard.hpp>

using namespace hal;

guard::color::color(lref<renderer> obj, hal::color c)
    : m_ref { obj }
    , m_old { m_ref->color() }
{
    set(c);
}

guard::color::~color()
{
    set(m_old);
}

void guard::color::set(hal::color c)
{
    m_ref->color(c);
}

guard::target::target(lref<renderer> obj, ref<target_texture> tx)
    : m_ref { obj }
{
    set(tx);
}

guard::target::~target()
{
    m_ref->reset_target();
}

void guard::target::set(ref<target_texture> tx)
{
    m_ref->target(tx);
}
