#pragma once
#include <list>
#include <queue>
#include <functional>
#include <thread>
#include <memory>
#include <mutex>

#include "Reactor/Socket/Handle.h"
#include "Reactor/Epoll/EpollPoller.h"
#include "Timer/Timer.h"

typedef std::function<void()>   PendingFunc;
typedef std::list<Handle>       HandleList;

class TimerQueue;

class EventLoop
{
private:
    std::list<std::shared_ptr<Handle>> m_handles;                   //测一下这个使用list合适还是map合适
    std::vector<std::shared_ptr<Handle>> m_handlesToRemove;
    std::queue<PendingFunc>         m_pendingFuncs;
    std::unique_ptr<EpollPoller>    m_poller;
    std::thread                     m_pollThread;
    bool                            m_runStatus;
    std::mutex                      m_mtx;
    std::thread::id                 m_loopThreadId;

    std::unique_ptr<TimerQueue>       m_pTimerQueue;
    
public:
    EventLoop();
    ~EventLoop();

    void run();
    void queueInLoop(const PendingFunc& func);

    void addHandles2Remove(std::shared_ptr<Handle> pHandle);

    void addhandle(std::shared_ptr<Handle> h);
    void removeHandle(std::shared_ptr<Handle> h);

    void runAt(const TImerFunc& func, uint32_t time);
    void runEvery(const TImerFunc& func, uint32_t mSec);
    void runAfter(const TImerFunc& func, uint32_t mSec);
};
