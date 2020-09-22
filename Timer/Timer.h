#pragma once
#include <stdint.h>
#include <functional>
#include <queue>
#include <mutex>
#include "Reactor/Socket/Handle.h"

typedef std::function<void()> TImerFunc;

class EventLoop;

class TimerNode
{
private:
    /* data */
    uint32_t    m_trigTime;
    bool        m_loop;
    uint32_t    m_interval;
    TImerFunc   m_func;
public:
    TimerNode(uint32_t trigTime,const TImerFunc& func, bool isLoop=false, uint32_t interval=0);
    ~TimerNode();

    TimerNode(const TimerNode& n);

    bool timeOut(const uint32_t &now);

    bool loop();

    bool operator > (TimerNode node)const;
    bool operator < (TimerNode node)const;
    TimerNode& operator = (const TimerNode& n);

    // loop 模式设置下次触发时间 
    void resetTrigTime(uint32_t t);
    void trig(EventLoop* pLoop);

};

class TimerQueue
{
private:
    std::priority_queue<TimerNode>  m_timers;
    int                             m_fd;
    EventLoop                       *m_pLoop;
    std::shared_ptr<Handle>         m_pHandle;
    std::mutex                      m_mtx;

public:
    TimerQueue(EventLoop* ploop);
    ~TimerQueue();

    void runAt(const TImerFunc& func, uint32_t time);
    void runEvery(const TImerFunc& func, uint32_t mSec);
    void runAfter(const TImerFunc& func, uint32_t mSec);

    void trig();
};