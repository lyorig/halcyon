#include <halcyon/utility/timer.hpp>

#include <ostream>

using namespace hal;

timer::timer()
    : m_epoch { clock::now() }
{
}

void timer::reset()
{
    m_epoch = clock::now();
}

double timer::operator()() const
{
    return std::chrono::duration<double> { elapsed() }.count();
}

timer::clock::duration timer::elapsed() const
{
    return clock::now() - m_epoch;
}

timer::clock::time_point timer::epoch() const
{
    return m_epoch;
}

std::ostream& hal::operator<<(std::ostream& str, timer t)
{
    return str << t() << 's';
}
