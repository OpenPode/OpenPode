#ifndef TIME_T_HPP_INCLUDED
#define TIME_T_HPP_INCLUDED

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

#include <ratio>
#include <chrono>

namespace std
{
    using minute = std::ratio<60,1>;
    using hour   = std::ratio<60*60,1>;
    using day    = std::ratio<24*60*60,1>;
    using week   = std::ratio<7*24*60*60,1>;
}

namespace util
{

template<typename T>
class time_t
{
private:

    using clock  = std::chrono::high_resolution_clock;

    using nano   = std::chrono::duration<T, std::nano>;
    using micro  = std::chrono::duration<T, std::micro>;
    using milli  = std::chrono::duration<T, std::milli>;
    using second = std::chrono::duration<T>;
    using minute = std::chrono::duration<T, std::minute>;
    using hour   = std::chrono::duration<T, std::hour>;
    using day    = std::chrono::duration<T, std::day>;
    using week   = std::chrono::duration<T, std::week>;

public:

    explicit time_t(clock::duration time);

    T nanos() const;
    T micros() const;
    T millis() const;
    T seconds() const;
    T minutes() const;
    T hours() const;
    T days() const;
    T weeks() const;

private:

    clock::duration m_time;
};

template<typename T>
time_t<T>::time_t(clock::duration time) : m_time(time) {}

template<typename T>
T time_t<T>::nanos() const { return  nano(m_time).count(); }

template<typename T>
T time_t<T>::micros() const
{
    return  micro(m_time).count();
}

template<typename T>
T time_t<T>::millis() const
{
    return  milli(m_time).count();
}

template<typename T>
T time_t<T>::seconds() const
{
    return  second(m_time).count();
}

template<typename T>
T time_t<T>::minutes() const
{
    return  minute(m_time).count();
}

template<typename T>
T time_t<T>::hours() const
{
    return  hour(m_time).count();
}

template<typename T>
T time_t<T>::days() const
{
    return  day(m_time).count();
}

template<typename T>
T time_t<T>::weeks() const
{
    return  week(m_time).count();
}


}

#endif // TIME_T_HPP_INCLUDED
