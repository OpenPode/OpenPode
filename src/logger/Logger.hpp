#ifndef LOGGER_HPP_INCLUDED
#define LOGGER_HPP_INCLUDED

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#if __cplusplus >= 201103L
#include <functional>
#endif

// TODO :
//      Add Log_level class ( m_log_level.str() instead of get_level_str() )

enum Log_level
{
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

class Logger
{
public:

    // Constructor
    Logger(Log_level min_log_level);

    // Destructor
    virtual ~Logger();

    // Public methods
    bool contain(std::ostream& out);

    // Members operator overloads
    Logger& operator+= (std::ostream& out);
    Logger& operator-= (std::ostream& out);
#if __cplusplus >= 201103L
    Logger& operator+= (std::vector< std::reference_wrapper<std::ostream> > outs);
    Logger& operator-= (std::vector< std::reference_wrapper<std::ostream> > outs);
#endif
    Logger& operator() (Log_level level);

    // No-members operator overloads
    template<typename T> friend
    Logger& operator<< (Logger& logger, T var);

protected:

    // Type redefinition
    using ostream_vect_t  = std::vector<std::ostream*>;
    using ostream_vect_it = std::vector<std::ostream*>::iterator;

    // Private methods
    std::string get_level_str() const;
    virtual void print(std::stringstream& message, size_t stream_index);

    // Private attributes
    ostream_vect_t  m_outs;
    Log_level       m_log_level;
    const Log_level m_min_log_level;
};

template<typename T>
Logger& operator<< (Logger& logger, T var)
{
    if(logger.m_log_level >= logger.m_min_log_level)
    {
        for(size_t i=0; i<logger.m_outs.size(); i++)
            if( logger.m_outs[i] != nullptr )
            {
                std::stringstream s;
                s << var;
                logger.print(s, i);
            }
    }
    return logger;
}

/* Code example

#include "Logger.hpp"
#include <fstream>

using namespace std;

int main()
{
    Logger log(INFO);
    ofstream f("test.log");

    log+=cout;
    log+=f;

    log(WARN) << "coucou";

    log-=cout;

    log(WARN) << "coucou 2";

    return 0;
}

output :

    [in standard output stream]
        WARN   : coucou

    [in test.log file]
        WARN   : coucou
        WARN   : coucou 2
*/
#endif // LOGGER_HPP_INCLUDED
