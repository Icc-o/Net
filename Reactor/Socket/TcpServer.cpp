#include "TcpServer.h"
#include <sys/epoll.h>
#include "Log/Logging.h"


TcpServer::TcpServer(std::string ip, int port, EventLoop* pLoop, int threadCount)
    :m_pLoop(pLoop)
    ,m_threadPool(threadCount)
{
    LOG_INFO << "Start TcpServer " << ip << ":" << port << ENDL;
    m_pSocket = std::make_shared<Socket>(::socket(AF_INET, SOCK_STREAM, 0)); 
    m_pSocket->setReuse();
    m_pHandle.reset(new Handle(m_pSocket->fd()));
    m_pHandle->setReadCb(std::bind(&TcpServer::HandleEvent, this, m_pSocket->fd()));
    m_pSocket->bind(ip, port);
}

TcpServer::~TcpServer()
{
}

void TcpServer::start()
{
    //m_pSocket->setNonblock();
    if(!m_pSocket->listen(1000))
    {
       LOG_WARN << "Listen error!" << ENDL;
       return;
    }
    m_pHandle->setPollEvents(EPOLLIN);
    if(m_pLoop)
        m_pLoop->addhandle(m_pHandle);   
}

void TcpServer::OnConnected(std::shared_ptr<Socket> pSock)
{
    //这些是不是都应该放到loop执行
    std::shared_ptr<Handle> pHandle = std::make_shared<Handle>(pSock->fd());
    pSock->setNonblock();
    pHandle->setPollEvents(EPOLLIN);
    pHandle->setReadCb(std::bind(&TcpServer::HandleEvent, this, pSock->fd()));
    m_pHandle->setDisconnectedCb(std::bind(&TcpServer::OnDisconnected, this, pSock->fd()));
    m_pLoop->addhandle(pHandle);

    m_clients.push_back(std::make_pair(pSock, pHandle));
    LOG_INFO << "TcpServer added client !" <<ENDL;
}

void TcpServer::OnDisconnected(int fd)
{
    auto itor = m_clients.begin();
    while(itor != m_clients.end())
    {
        if((*itor).first->fd()== fd)
        {
            m_pLoop->addHandles2Remove((*itor).second);
            LOG_INFO << fd <<" disconnected" << " Reference count: "<< (*itor).first.use_count()<<ENDL;
            m_clients.erase(itor);
            break;
        }
        itor++;
    }
}

void TcpServer::OnRecvMsg(std::shared_ptr<Socket> pSock, std::string buff)
{
    if(m_recvCb)
        m_recvCb(pSock, buff.c_str());
    //else
    //{
        LOG_INFO <<pSock->fd() <<" recv : "<<buff<<ENDL;
    //}
}

void TcpServer::HandleEvent(const int &fd)
{
    if(fd == m_pSocket->fd())           //接收连接的fd
    {
        std::string addr;
        std::shared_ptr<Socket> pSock = m_pSocket->accept(addr);
        if(pSock)
        {
            LOG_INFO << "Add client "<<pSock->fd() <<ENDL;
            OnConnected(pSock);
        }
    }
    else
    {
        for(auto &s : m_clients)
        {
            std::string msg;
            if((s.first) && (s.first->fd() == fd))
            {
                int ret = s.first->recv(msg);
                if(ret>0)
                    //OnRecvMsg(s.first, msg.c_str());  接收数据处理放到线程池
                    m_threadPool.add_work(std::bind(&TcpServer::OnRecvMsg, this, s.first, msg));
                else{
                    OnDisconnected(s.first->fd());
                    //m_threadPool.add_work(std::bind(&TcpServer::OnDisconnected, this, s.first->fd()));
                    break;
                }
            }
        }
    }
}