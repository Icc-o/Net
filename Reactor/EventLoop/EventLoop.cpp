#include "EventLoop.h"
#include "Timer/Timer.h"

EventLoop::EventLoop()
    :m_runStatus(true)
    ,m_pollThread(&EventLoop::run, this)
{
    m_poller = std::make_unique<EpollPoller>();
    m_pTimerQueue = std::make_unique<TimerQueue>(this);
}

EventLoop::~EventLoop()
{
    m_runStatus = false;
    if(m_pollThread.joinable())
        m_pollThread.join();
}

void EventLoop::run()
{
    m_loopThreadId = std::this_thread::get_id();
    m_runStatus = true;
    while(m_runStatus)
    {
        if(!m_handles.empty() && m_poller)
            m_poller->Poll(m_handles);
    
        for(auto& h : m_handles)
            h->handleActiveEvent();     //不在这里删除handles，因为会导致迭代器失效
    
        for(auto &h : m_handlesToRemove)
            removeHandle(h);
        m_handlesToRemove.clear();
        
        while(!m_pendingFuncs.empty())
        {
            //如果把入队列的函数放到这里那这个循环就出不去了
            m_pendingFuncs.front()();
            std::lock_guard<std::mutex> lk(m_mtx);
            m_pendingFuncs.pop();
        }
    }
}

void EventLoop::queueInLoop(const PendingFunc& func)
{
    std::lock_guard<std::mutex> lk(m_mtx);
    m_pendingFuncs.push(func);
}

void EventLoop::addhandle(std::shared_ptr<Handle> h)
{
        m_handles.push_back(h);
        m_poller->AddHandleEvent(h);
}

void EventLoop::removeHandle(std::shared_ptr<Handle> h)
{
    auto itor = m_handles.begin();
    while(itor != m_handles.end())
    {
        if(*itor == h)
        {
            m_handles.erase(itor);
            break;
        }
        itor++;
    }
}

void EventLoop::addHandles2Remove(std::shared_ptr<Handle> pHandle)
{
    m_handlesToRemove.push_back(pHandle);
    m_poller->RemoveHandleEvent(pHandle);
}


void EventLoop::runAt(const TImerFunc& func, uint32_t time)
{
    m_pTimerQueue->runAt(func,time);
}


void EventLoop::runEvery(const TImerFunc& func, uint32_t mSec)
{
    m_pTimerQueue->runEvery(func, mSec);
}

void EventLoop::runAfter(const TImerFunc& func, uint32_t mSec)
{
    m_pTimerQueue->runAfter(func, mSec);
}