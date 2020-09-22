#include "EpollPoller.h"
#include <string.h>
#include "Log/Logging.h"
#include "EventLoop.h"

EpollPoller::EpollPoller(const int& maxEventCount)
    :m_epollFd(epoll_create1(0))
    ,m_maxEvent(maxEventCount)
{
    //m_events = (epoll_event*)malloc(sizeof(epoll_event)*m_maxEvent);
    m_events = new epoll_event[m_maxEvent];
}

EpollPoller::~EpollPoller()
{

}

void EpollPoller::AddHandleEvent(std::shared_ptr<Handle> h)
{
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = h->fd();
    ev.events = h->pollEvents();

    int ret = epoll_ctl(m_epollFd, EPOLL_CTL_ADD, h->fd(), &ev);
    if(ret == -1)
        LOG_WARN << "Epoll_Ctl failed! "<<ENDL;
}

void EpollPoller::RemoveHandleEvent(std::shared_ptr<Handle> h)
{
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = h->fd();
    ev.events = h->pollEvents();

    int ret = epoll_ctl(m_epollFd, EPOLL_CTL_DEL, h->fd(), &ev);
    if(ret == -1)
        LOG_WARN << "Epoll_Ctl failed! "<< strerror(errno) <<ENDL;
}

void EpollPoller::Poll(std::list<std::shared_ptr<Handle>>& handles)
{
    int nCount = epoll_wait(m_epollFd, m_events, m_maxEvent, 1000);
    if(nCount<0)
    {
        if(errno!=EINTR)
        {
            LOG_WARN << "epoll_wait error: "<<strerror(errno) <<ENDL;
            return;
        }
    }
    SetEventsTOHandles(handles, nCount);
}

void EpollPoller::SetEventsTOHandles(std::list<std::shared_ptr<Handle>>& handles, int nEvent)
{
    for(int i=0; i<nEvent; i++)
    {
        uint32_t events = m_events[i].events;
        
        //TODO : 优化遍历 （可以使用map<int, handle>）
        for(auto &h : handles)
        {
            if(h->fd() == m_events[i].data.fd)
            {
                h->setCurrentEvent(events);
            }
        }
    }
}