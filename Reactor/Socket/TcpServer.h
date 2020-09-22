#pragma once
#include <memory>
#include <list>

#include "Reactor/Socket/Handle.h"
#include "Reactor/Socket/Socket.h"
#include "Reactor/EventLoop/EventLoop.h"
#include "ThreadPool/ThreadPool.h"


typedef std::function<void(const int& fd)> ConnectCallback;
typedef std::function<void(std::shared_ptr<Socket>, const char* buff)> RecvCallback;

class TcpServer
{
private:
    std::shared_ptr<Handle> m_pHandle;
    std::shared_ptr<Socket> m_pSocket;
    std::list<std::pair<std::shared_ptr<Socket>, std::shared_ptr<Handle>>> m_clients;
    EventLoop                               *m_pLoop;

    ConnectCallback         m_connectedCb, m_disconnectedCb;
    RecvCallback            m_recvCb;

    //处理接收数据的线程池
    ThreadPool              m_threadPool;

public:
    TcpServer(std::string ip, int port, EventLoop *pLoop, int threadCount=4);
    ~TcpServer();
    void start();

    void HandleEvent(const int &fd);
    void SetConnectedCb(ConnectCallback &cb){m_connectedCb = std::move(cb);}
    void SetDisconnectedCb(ConnectCallback &cb){m_disconnectedCb = std::move(cb);}
    void SetRecvCb(const RecvCallback &cb){m_recvCb = std::move(cb);}

private:
    void OnConnected(std::shared_ptr<Socket> pSock);
    void OnDisconnected(int fd);
    void OnRecvMsg(std::shared_ptr<Socket> pSock , std::string buff);
};