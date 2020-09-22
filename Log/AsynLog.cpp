#include "AsynLog.h"
#include <iostream>
#include <sstream>

static std::once_flag oc;
std::shared_ptr<AsynLog> AsynLog::m_pAsLog;

extern std::string levelStr[];

AsynLog::AsynLog(const std::string &name)
    :m_currentBuffer(std::make_shared<LogBuffer<LOG_BUFFER_SIZE>>())
    ,m_nextBuffer(std::make_shared<LogBuffer<LOG_BUFFER_SIZE>>())
    ,m_endBUffer(std::make_shared<LogBuffer<LOG_BUFFER_SIZE>>())
    ,m_proName(name)
{
    m_thread = std::move(std::thread(&AsynLog::run, this));
    if(m_proName.empty())
        m_proName="Server";
}

AsynLog::~AsynLog()
{
    m_runStatus = false;
    m_cnd.notify_one();
    if(m_thread.joinable())
        m_thread.join();
}

std::shared_ptr<AsynLog> AsynLog::getInstance(const std::string &name)
{
    std::call_once(oc, [&](){m_pAsLog = std::make_shared<AsynLog>(name);});
    return m_pAsLog;
}

 void AsynLog::push(const char* msg)
 {
     std::lock_guard<std::mutex> lk(m_mtx);
    if(m_currentBuffer->avail() >= strlen(msg))
        m_currentBuffer->push(msg,strlen(msg));
    else if(m_nextBuffer)
    {
        //通知线程写入到文件
        //m_currentBuffer = std::move(m_nextBuffer);
        m_reserveBuffers.push(m_currentBuffer);
        m_currentBuffer = std::move(m_nextBuffer);
        m_currentBuffer->push(msg,strlen(msg));
        m_cnd.notify_one();
    }
    else
    {
        //std::cout<<"Not happen place!"<<std::endl;
        m_reserveBuffers.push(m_currentBuffer);
        m_currentBuffer.reset();
        m_currentBuffer = std::make_shared<LogBuffer<LOG_BUFFER_SIZE>>();
        m_currentBuffer->push(msg, strlen(msg));
        m_cnd.notify_one();
    }
    
 }

void AsynLog::run()
{
    m_runStatus = true;
    LogFile logFile(m_proName.c_str());
    while(m_runStatus)
    {
        {
            //锁只需要保护到把buffer放到了endBUffer
            std::unique_lock<std::mutex> lk(m_mtx);

            if(m_reserveBuffers.empty() && m_currentBuffer->empty())
            {
                //m_cnd.wait(lk);
                m_cnd.wait_for(lk, std::chrono::seconds(2));    //强行2s刷一次
            }
            
            if(!m_reserveBuffers.empty())
            {
                if(!m_nextBuffer)
                {
                    m_nextBuffer = std::move(m_endBUffer);
                }
                //m_endBUffer = std::move(m_reserveBuffers.front());
                m_endBUffer = std::move(m_reserveBuffers.front());
                m_reserveBuffers.pop();
            }
            else if(!m_currentBuffer->empty())
            {
                m_currentBuffer.swap(m_endBUffer);
            }
        }

        if(!m_endBUffer->empty())
        {
            logFile.append(m_endBUffer->data(), m_endBUffer->len());
            logFile.flush();
            m_endBUffer->clear();
        }
    }

    //再把没写完的写到文件中去
    if(!m_currentBuffer->empty())
    {
        logFile.append(m_currentBuffer->data(), m_currentBuffer->len());
        m_currentBuffer->clear();
    }
    if(!m_nextBuffer->empty())
    {
        logFile.append(m_nextBuffer->data(), m_nextBuffer->len());
        m_nextBuffer->clear();
    }
    while(!m_reserveBuffers.empty())
    {
        logFile.append(m_reserveBuffers.front()->data(), m_reserveBuffers.front()->len());
        m_reserveBuffers.pop();
    }
}



