#pragma once
#include "EventLoop.h"
#include "TcpServer.h"

class HttpServer
{
private:
    /* data */
    std::unique_ptr<TcpServer>  m_pTcpServer;
    EventLoop*                  m_pLoop;

public:
    HttpServer(EventLoop* pLoop=nullptr);
    ~HttpServer();

    void OnMessage(std::shared_ptr<Socket> sock, const char* buff);
};
