#include <memory>
#include <mutex>
#include <functional>
#include "LogStream.h"

//这个宏每次调用的时候生成一个临时的Logger对象，写入之后在析构中调用设置的writeFunc
//所以好几个成员都是static的

//这里不是单例哦！
#define LOG_INFO if(Logger::LOGL_INFO == Logger::m_logLevel) Logger(__FILE__, __LINE__, Logger::LOGL_INFO).m_stream
#define LOG_WARN if(Logger::LOGL_WARN >= Logger::m_logLevel) Logger(__FILE__, __LINE__, Logger::LOGL_WARN).m_stream
#define LOG_ERR if(Logger::LOGL_ERR >= Logger::m_logLevel) Logger(__FILE__, __LINE__, Logger::LOGL_ERR).m_stream

#define ENDL "\n"

typedef std::function<void(const char* data)> LogWriteFunc;

enum{
    LOGL_INFO = 0,
    LOGL_WARN = 1,
    LOGL_ERR  = 2,
};

class Logger
{
public:
    enum{
        LOGL_INFO = 0,
        LOGL_WARN = 1,
        LOGL_ERR  = 2,
    };
    Logger(const char* fileName, int line, int level);
    ~Logger();

    static void setWriteFunc(LogWriteFunc func);
    static void setLogLevel(int logLevel);
    static void setLogName(const std::string& name);

    LogStream m_stream;

    static int                          m_logLevel;
    static LogWriteFunc                 m_writeFunc;
    std::string                         m_headStr;      //日志的头字符串(time + function + line)
};