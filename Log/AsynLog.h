#pragma once
#include "LogStream.h"
#include <thread>
#include <condition_variable>
#include <queue>

class AsynLog
{
    typedef std::shared_ptr<LogBuffer<LOG_BUFFER_SIZE>> BufferPtr;
public:
    AsynLog(const std::string &name);
    ~AsynLog();

    AsynLog(const AsynLog&) = delete;
    const AsynLog& operator = (const AsynLog&)=delete;

    void setProgramName(const std::string &name){m_proName=name;}
    static std::shared_ptr<AsynLog> getInstance(const std::string &name="");

    void push(const char* msg);
private:
    void run();

private:
    static std::shared_ptr<AsynLog> m_pAsLog;
    std::thread                     m_thread;

    BufferPtr   m_currentBuffer;    //当前日志写入的buffer   
    BufferPtr   m_nextBuffer;       //给当前buffer当备用
    BufferPtr   m_endBUffer;        //后台的buffer
    std::queue<BufferPtr> m_reserveBuffers;      //缓存

    std::mutex  m_mtx;              //保护插入
    std::condition_variable m_cnd;
    bool        m_runStatus;
    std::string m_proName;          //程序名
};