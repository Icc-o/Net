#include "Timer.h"
#include <sys/timerfd.h>
#include <sys/time.h>
#include <unistd.h>
#include "Log/Logging.h"
#include "EventLoop.h"


TimerNode::TimerNode(uint32_t trigTime, const TImerFunc& func, bool isLoop, uint32_t interval)
    :m_trigTime(trigTime)
    ,m_loop(isLoop)
    ,m_interval(interval)
    //,m_func(std::move(func))
    ,m_func(func)
{
}

TimerNode::~TimerNode()
{
}

TimerNode::TimerNode(const TimerNode& n)
{
    this->m_trigTime = n.m_trigTime;
    this->m_loop = n.m_loop;
    this->m_func = n.m_func;
    this->m_interval = n.m_interval;
}

bool TimerNode::timeOut(const uint32_t &now)
{
    return now > m_trigTime;
}

bool TimerNode::loop()
{
    return m_loop;
}

bool TimerNode::operator > (TimerNode node) const
{
    return this->m_trigTime > node.m_trigTime;
}

bool TimerNode::operator < (TimerNode node) const
{
    return this->m_trigTime < node.m_trigTime;
}

TimerNode& TimerNode::operator = (const TimerNode& n)
{
    this->m_trigTime = n.m_trigTime;
    this->m_loop = n.m_loop;
    this->m_func = n.m_func;
    this->m_interval = n.m_interval;
}

    // loop 模式设置下次触发时间 
void TimerNode::resetTrigTime(uint32_t t)
{
    m_trigTime=t+m_interval;
} 
    
void TimerNode::trig(EventLoop* pLoop)
{
    this->m_func();               //可以直接在这里执行，因为是EventLoop中的Handle触发事件调过来的
}

TimerQueue::TimerQueue(EventLoop* ploop)
    :m_fd(timerfd_create(CLOCK_REALTIME, 0))
    ,m_pLoop(ploop)
{   
    struct timeval now;
    gettimeofday(&now, NULL);

    struct itimerspec new_value;
    //第一次超时的时间
    new_value.it_value.tv_sec = now.tv_sec+2;
    new_value.it_value.tv_nsec = 0;
    //后续周期的时间
    new_value.it_interval.tv_sec = 1;
    new_value.it_interval.tv_nsec = 0;
    if(-1 == timerfd_settime(m_fd, TFD_TIMER_ABSTIME, &new_value, nullptr))
    {
        LOG_INFO<<"settime failed!"<<ENDL;
    }

    m_pHandle = std::make_shared<Handle>(m_fd);
    m_pHandle->setReadCb(std::bind(&TimerQueue::trig, this));
    m_pHandle->setPollEvents(EPOLLIN);
    m_pLoop->addhandle(m_pHandle);
}

TimerQueue::~TimerQueue()
{

}


void TimerQueue::trig()
{
    uint64_t content;
    read(m_fd, &content, sizeof(content));
    if(m_timers.empty())
    {
        //LOG_INFO << "Timer fd triggered! "<<ENDL;
        return;
    }
    struct timeval now;
    gettimeofday(&now, nullptr);
    uint32_t mSec = now.tv_sec*1000 + now.tv_usec/1000;

    TimerNode topNode = m_timers.top();
    while(topNode.timeOut(mSec))
    {
        topNode.trig(m_pLoop);
        m_timers.pop();
        if(topNode.loop())
        {
            topNode.resetTrigTime(mSec);
            m_timers.push(topNode);
        }
        if(m_timers.empty())
            break;
        topNode = m_timers.top();
    }
}

void TimerQueue::runAt(const TImerFunc& func, uint32_t time)
{
    TimerNode node(time,func);
    std::lock_guard<std::mutex> lk(m_mtx);
    m_timers.push(node);
}

void TimerQueue::runEvery(const TImerFunc& func, uint32_t mSec)
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    uint32_t time = now.tv_sec*1000 + now.tv_usec/1000 + mSec;
    TimerNode node(time,func,true,mSec);
    std::lock_guard<std::mutex> lk(m_mtx);
    m_timers.push(node);
}


void TimerQueue::runAfter(const TImerFunc& func, uint32_t mSec)
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    uint32_t time = now.tv_sec*1000 + now.tv_usec/1000 + mSec;
    TimerNode node(time,func);
    std::lock_guard<std::mutex> lk(m_mtx);
    m_timers.push(node);
}