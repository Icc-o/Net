#pragma once
#include <sys/epoll.h>
#include <list>
#include "Reactor/Socket/Handle.h"

class PollBase{
public:
    PollBase(){}
    virtual ~PollBase(){}

    virtual void AddHandleEvent(std::shared_ptr<Handle> h)=0;
    virtual void RemoveHandleEvent(std::shared_ptr<Handle> h) = 0;
    virtual void Poll(std::list<std::shared_ptr<Handle>>& handles)=0;
};

class EpollPoller:public PollBase
{
private:
    /* data */
    const int m_epollFd;
    const int m_maxEvent;
    struct epoll_event  *m_events;

    //std::vector<TcpConnection> m_clients;
public:
    EpollPoller(const int& maxEventCount=20);
    ~EpollPoller();

    void AddHandleEvent(std::shared_ptr<Handle> h) override;
    void RemoveHandleEvent(std::shared_ptr<Handle> h) override;
    void Poll(std::list<std::shared_ptr<Handle>>& handles) override;

    void SetEventsTOHandles(std::list<std::shared_ptr<Handle>>& handles, int nEvent);
};
