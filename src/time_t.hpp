#ifndef TIME_T_HPP_INCLUDED
#define TIME_T_HPP_INCLUDED

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

#include <ratio>
#include <chrono>

namespace std
{
    typedef std::ratio<60,1>            minute;
    typedef std::ratio<60*60,1>         hour;
    typedef std::ratio<24*60*60,1>      day;
    typedef std::ratio<7*24*60*60,1>    week;
}

namespace util
{

class time_t
{
private:

    using clock = std::chrono::high_resolution_clock;

    using nano = std::chrono::duration<double, std::nano>;
    using micro = std::chrono::duration<double, std::micro>;
    using milli = std::chrono::duration<double, std::milli>;
    using second = std::chrono::duration<double>;
    using minute = std::chrono::duration<double, std::minute>;
    using hour = std::chrono::duration<double, std::hour>;
    using day = std::chrono::duration<double, std::day>;
    using week = std::chrono::duration<double, std::week>;

public:

    explicit time_t(clock::duration time);

    double nanos() const;
    double micros() const;
    double millis() const;
    double seconds() const;
    double minutes() const;
    double hours() const;
    double days() const;
    double weeks() const;

private:

    clock::duration m_time;
};

}

#endif // TIME_T_HPP_INCLUDED
