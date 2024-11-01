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

f64 timer::operator()() const
{
    return std::chrono::duration<f64> { clock::now() - m_epoch }.count();
}

timer::clock::time_point timer::time_point() const
{
    return m_epoch;
}

std::ostream& hal::operator<<(std::ostream& str, timer t)
{
    return str << t() << 's';
}
