#include <halcyon/utility/guard.hpp>

using namespace hal;

guard::lock::lock(lref<streaming_texture> tex)
    : m_ref { tex }
    , res { tex->lock() }
{
}

guard::lock::lock(lref<streaming_texture> tex, pixel::rect area)
    : m_ref { tex }
    , res { tex->lock(area) }
{
}

guard::lock::~lock()
{
    m_ref->unlock();
}

guard::color::color(lref<renderer> obj, hal::color c)
    : m_ref { obj }
    , m_old { m_ref->color().get() }
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

guard::presentation::presentation(lref<renderer> rnd, pixel::point sz, renderer::scaling scl)
    : m_ref { rnd }
    , m_old { rnd->presentation().get() }
{
    set(sz, scl);
}

guard::presentation::~presentation()
{
    set(m_old.first, m_old.second);
}

void guard::presentation::set(pixel::point sz, renderer::scaling scl)
{
    m_ref->presentation(sz, scl);
}
