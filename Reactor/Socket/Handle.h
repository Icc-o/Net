#pragma once
#include <functional>
#include <memory>
#include "Socket.h"


typedef std::function<void()>   HandleCallBack;

enum class EventType{
    EVENT_NOEVENT=0,
    EVENT_CONNECTED,        
    EVENT_DISCONNECTED,
    EVENT_RECV,
    EVENT_SEND,
    EVENT_TIMEOUT,
};

class Handle
{
private:
    /* data */
    HandleCallBack     m_disconnectedFunc;
    HandleCallBack     m_readFunc;

    int                 m_fd;
    EventType               m_type;

    uint32_t                m_pollEvent;    //待�?�理的事�?
    uint32_t                m_currentEvent;

public:
    Handle(int fd);
    ~Handle();
    
    void setDisconnectedCb(const HandleCallBack &func){m_disconnectedFunc = std::move(func);}
    void setReadCb(const HandleCallBack &func){m_readFunc = std::move(func);}

    void setActiveEvent(const EventType &type);
    void handleActiveEvent();

    bool operator == (const Handle& h);

    int fd();
    int pollEvents(){return m_pollEvent;}
    void setPollEvents(uint32_t pollEve){m_pollEvent = pollEve;}             //��Ҫ���ӵ��¼�
    void setCurrentEvent(uint32_t pollEvent){m_currentEvent = pollEvent;}    //��ǰ�������¼�
};


