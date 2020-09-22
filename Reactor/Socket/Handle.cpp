#include "Handle.h"
#include <sys/epoll.h>
#include "DefFunc.h"

Handle::Handle(int fd)
    :m_readFunc(recv_func)
    ,m_disconnectedFunc(disconnected_func)
    ,m_type(EventType::EVENT_NOEVENT)
    ,m_currentEvent(0)
    ,m_fd(fd)
{
}

Handle::~Handle()
{
}

void Handle::setActiveEvent(const EventType & type)
{
    m_type = type;
}

void Handle::handleActiveEvent()
{
    //这里还需要区分事件类型再调用对应的回调
    if(!m_currentEvent)
        return;

    if(m_currentEvent&EPOLLERR || m_currentEvent&EPOLLHUP || (!m_currentEvent&EPOLLIN))
    {
        LOG_INFO <<"error event!" << ENDL;
        if(m_disconnectedFunc)
            m_disconnectedFunc();
    }
    //else
    //{
        if(m_readFunc)
            m_readFunc();
        else
        {
            LOG_INFO << "Connected Func not registered!" << ENDL;
        }
        
    //}
    m_currentEvent = 0;
}

bool Handle::operator == (const Handle& h)
{
    if(this == &h)
        return true;
    else
        return (this->m_fd == h.m_fd);
}

int Handle::fd()
{
    return m_fd;
}