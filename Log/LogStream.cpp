#include "LogStream.h"
//#include <string>


template<int SIZE>
bool LogBuffer<SIZE>::push(const char* buff, uint32_t len)
{
    if(this->avail() >= len)
    {
        memmove(m_buff+m_curPos, buff, len);
        m_curPos+=len;
    }
    else
        return false;
    return true;
}


LogStream::LogStream()
{
    m_pBuffer.reset(new LogBuffer<LOG_BUFFER_SIZE>);
}

LogStream& LogStream::operator<<(const std::string &content)
{
    m_pBuffer->push(content.c_str(), content.size());
    //m_pBuffer->push("\n", 1);
    return *this;
}


LogStream& LogStream::operator << (int content)
{
    std::string str = std::move(std::to_string(content));
    return operator << (str);
}

LogStream& LogStream::operator << (double content)
{
    std::string str = std::move(std::to_string(content));
    return operator << (str);
}

LogStream& LogStream::operator << (const char* content)
{
    std::string str(content);
    return operator << (str);
}