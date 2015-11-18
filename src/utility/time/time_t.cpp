#include "time_t.hpp"

namespace util
{

time_t::time_t(clock::duration time) : m_time(time)
{

}

double time_t::nanos() const
{
    return  nano(m_time).count();
}

double time_t::micros() const
{
    return  micro(m_time).count();
}

double time_t::millis() const
{
    return  milli(m_time).count();
}

double time_t::seconds() const
{
    return  second(m_time).count();
}

double time_t::minutes() const
{
    return  minute(m_time).count();
}

double time_t::hours() const
{
    return  hour(m_time).count();
}

double time_t::days() const
{
    return  day(m_time).count();
}

double time_t::weeks() const
{
    return  week(m_time).count();
}

}
