#ifndef TIMER_T_HPP_INCLUDED
#define TIMER_T_HPP_INCLUDED

#include "time_t.hpp"

namespace util
{

template<typename T>
class timer_t
{
    using clock = std::chrono::high_resolution_clock;

public:

    explicit timer_t(bool run = false);

    void reset();

    time_t<T> elapsed() const;

private:

    clock::time_point m_start;
};

template<typename T>
timer_t<T>::timer_t(bool run)
{
    if (run)
        reset();
}

template<typename T>
void timer_t<T>::reset()
{
    m_start = clock::now();
}

template<typename T>
time_t<T> timer_t<T>::elapsed() const
{
    return time_t<T>(clock::now() - m_start);
}

}

#endif // TIMER_T_HPP_INCLUDED
