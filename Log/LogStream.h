#pragma once
#include <string.h>
#include <memory>
#include "LogFile.h"

const int LOG_BUFFER_SIZE = 4096;

//封装char数组，提供几个操作的接口
template<int SIZE>
class LogBuffer
{
public:
    LogBuffer():m_curPos(0){}
    ~LogBuffer(){}

    bool push(const char* buff, uint32_t len);
    void clear(){m_curPos = 0; bzero(m_buff, SIZE);}
    uint32_t avail()const{return SIZE-m_curPos;}
    uint32_t len()const{return m_curPos;}
    char* data(){return m_buff;}
    bool empty()const{return m_curPos == 0;}

    LogBuffer(const LogBuffer&) = delete;
    const LogBuffer& operator = (const LogBuffer& ) = delete;

private:
    char        m_buff[SIZE];
    uint32_t    m_curPos;
};

//封装一层Buffer，提供 << 操作符
class LogStream
{
public:
    LogStream();
    ~LogStream(){}

    //后续可以考虑实现
    LogStream(const LogStream& s) = delete;
    const LogStream& operator = (const LogStream& s)=delete;

    LogStream& operator << (const std::string& content);
    LogStream& operator << (int content);
    LogStream& operator << (double content);
    LogStream& operator << (const char* content);

    const char* data(){return m_pBuffer->data();}
    void clear(){m_pBuffer->clear();}

private:
    std::unique_ptr<LogBuffer<LOG_BUFFER_SIZE>> m_pBuffer;
};