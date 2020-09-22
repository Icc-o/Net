#include "Logging.h"
#include <iostream>
#include <sstream>
#include "Util/Util.h"
#include "AsynLog.h"

std::string levelStr[] = {"[INFO]", "[WARN]", "ERR"};

//默认打到异步日志里面去
void defWriteLog(const char* buffer)
{
    AsynLog::getInstance()->push(buffer);
}

int Logger::m_logLevel = Logger::LOGL_INFO;
LogWriteFunc Logger::m_writeFunc=defWriteLog;

Logger::Logger(const char* fileName, int line, int level)
{
    std::ostringstream oss;
    oss<<levelStr[level]<<GetFormatedTime()<<" "<<fileName<<":"<<line<<" ";
    m_stream << oss.str();
}

Logger::~Logger()
{
    m_writeFunc(m_stream.data());
    m_stream.clear();
}

void Logger::setWriteFunc(LogWriteFunc func)
{
    m_writeFunc = func;
}

void Logger::setLogLevel(int logLevel)
{
    if(logLevel>=LOGL_INFO && logLevel<=LOGL_ERR)
        m_logLevel =  logLevel;
    else
    {
        LOG_WARN << "Error Log level " <<logLevel<<ENDL;
    }
}

void Logger::setLogName(const std::string& name)
{
    AsynLog::getInstance(name);
}