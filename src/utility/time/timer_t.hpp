#ifndef TIMER_T_HPP_INCLUDED
#define TIMER_T_HPP_INCLUDED

#include "time_t.hpp"

namespace util
{

class timer_t
{
    using clock = std::chrono::high_resolution_clock;

public:

    explicit timer_t(bool run = false);

    void reset();

    time_t elapsed() const;

private:

    clock::time_point m_start;
};

}

#endif // TIMER_T_HPP_INCLUDED
