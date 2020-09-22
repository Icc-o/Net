#include "ThreadPool.h"

ThreadPool::ThreadPool(short count):m_runStatus(true)
{
    for(short i=0; i<count; i++)
    {
        m_threads.emplace_back(std::thread([&](){
            while(m_runStatus)
        {
            ThreadFunc func;
            {
                std::unique_lock<std::mutex> lk(m_mtx);
                m_cnd.wait(lk, [&](){return (!m_runStatus || !m_works.empty());});
                func = std::move(m_works.front());
                m_works.pop();
            }
            func();
        }
        }));
    }
}

ThreadPool::~ThreadPool()
{
    m_runStatus = false;
    m_cnd.notify_all();
    for(auto &t : m_threads)
        if(t.joinable())
            t.join();
}

void ThreadPool::add_work(const ThreadFunc& func)
{
    std::lock_guard<std::mutex> lk(m_mtx);
    m_works.push(func);
    m_cnd.notify_one();
}