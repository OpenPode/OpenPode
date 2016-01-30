#ifndef FILELOGGER_HPP_INCLUDED
#define FILELOGGER_HPP_INCLUDED

#include "Logger.hpp"
#include <fstream>

class FileLogger : public Logger
{
public:
    FileLogger(const std::string& filename, Log_level min_log) : Logger(min_log), m_file(filename, std::ofstream::app)
    {
        *this += m_file;
        *this += std::cout;
    }

    ~FileLogger()
    {
        m_file.close();
    }

protected:

    std::ofstream m_file;
};

#endif // FILELOGGER_HPP_INCLUDED
