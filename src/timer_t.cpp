#include "timer_t.hpp"

namespace util
{

timer_t::timer_t(bool run)
{
    if (run)
        reset();
}

void timer_t::reset()
{
    m_start = clock::now();
}

time_t timer_t::elapsed() const
{
    return time_t(clock::now() - m_start);
}

}
