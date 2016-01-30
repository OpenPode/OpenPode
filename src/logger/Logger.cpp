#include "Logger.hpp"
#include <algorithm>

Logger::Logger(Log_level min_log_level) : m_outs(), m_log_level(min_log_level), m_min_log_level(min_log_level)
{

}

Logger::~Logger()
{

}

bool Logger::contain(std::ostream& out)
{
    return std::find(m_outs.begin(), m_outs.end(), &out) == m_outs.end() ? false : true;
}

Logger& Logger::operator+= (std::ostream& out)
{
    ostream_vect_it it = std::find(m_outs.begin(), m_outs.end(), &out);
    if(it == m_outs.end())
        m_outs.push_back(&out);
    return *this;
}

Logger& Logger::operator-= (std::ostream& out)
{
    ostream_vect_it it = std::find(m_outs.begin(), m_outs.end(), &out);
    if(it != m_outs.end())
        m_outs.erase(it);
    return *this;
}

#if __cplusplus >= 201103L

Logger& Logger::operator+= (std::vector< std::reference_wrapper<std::ostream> > outs)
{
    for(size_t i=0 ; i<outs.size() ; i++)
        *this += outs[i];
    return *this;
}

Logger& Logger::operator-= (std::vector< std::reference_wrapper<std::ostream> > outs)
{
    for(size_t i=0 ; i<outs.size() ; i++)
        *this -= outs[i];
    return *this;
}

#endif

Logger& Logger::operator()(Log_level level)
{
    m_log_level = level;
    return *this;
}

std::string Logger::get_level_str() const
{
    switch(m_log_level)
    {
    case TRACE : return "TRACE";
    case DEBUG : return "DEBUG";
    case INFO  : return "INFO ";
    case WARN  : return "WARN ";
    case ERROR : return "ERROR";
    case FATAL : return "FATAL";
    default    : return "UNKNW";
    }
}

void Logger::print(std::stringstream& message, size_t stream_index)
{
    *(m_outs[stream_index]) << "> [" << get_level_str() << "] : " << message.str() << "\n\r";
}
