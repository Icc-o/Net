#pragma once
#include <thread>
#include <functional>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <queue>

typedef std::function<void()> ThreadFunc;

//线程池类
class ThreadPool
{
public:
    ThreadPool(short count);
    ~ThreadPool();
    void add_work(const ThreadFunc& func);

private:
    std::queue<ThreadFunc>   m_works;
    int                      m_threadCount;
    std::vector<std::thread> m_threads;
    std::mutex               m_mtx;
    std::condition_variable  m_cnd;
    bool                     m_runStatus;
};