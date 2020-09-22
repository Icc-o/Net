#include "Socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <error.h>
#include <unistd.h>
#include <fcntl.h>

#include "Log/Logging.h"

Socket::Socket(const int &fd):m_fd(fd)
{
}

Socket::~Socket()
{
    LOG_INFO <<"close fd "<<m_fd<<ENDL;
    //close();
    ::close(m_fd);  //在析构的地方必须使用close确保文件描述符关闭
}

bool Socket::bind(const std::string &ip, const int &flag)
{
    struct sockaddr_in listenAddr;
    bzero(&listenAddr, sizeof(listenAddr));
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_port = htons(flag);
    listenAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //inet_pton(AF_INET, ip.c_str(), &listenAddr.sin_addr);
    
    if(::bind(m_fd, (struct sockaddr*)&listenAddr, sizeof(listenAddr))<0)
    {
        LOG_WARN << "Bind "<<ip<<" failed! Cause: "<<strerror(errno)<<ENDL;
        return false;
    }
    LOG_INFO << "Bind "<<ip<< " good "<<ENDL;
    return true;
}

bool Socket::listen(const int maxConn)
{
    if(::listen(m_fd, maxConn) !=0)
    {
        LOG_WARN << "Listen error!" << ENDL;
        return false;
    }
    LOG_WARN << "Listen "<< " good "<<ENDL;
    return true;
}

std::shared_ptr<Socket> Socket::accept(std::string& addr)
{
    struct sockaddr_in clientAddr;
    socklen_t size = sizeof(clientAddr);
    int fd =::accept(m_fd, (struct sockaddr*)&clientAddr, &size);
    if(fd <= 0)
    {
        LOG_WARN << "accept error, reason: "<<strerror(errno) <<ENDL;
        return nullptr;
    }
    LOG_INFO << "accept client "<<fd <<ENDL;

    char tmpAddr[20];
    inet_ntop(AF_INET, &clientAddr, tmpAddr, 20);
    addr = tmpAddr;
    addr += std::to_string(ntohs(clientAddr.sin_port));
    return std::make_shared<Socket>(fd);
}

bool Socket::connect(const std::string &ip, const int &port)
{
    struct sockaddr_in servreAddr;
    bzero(&servreAddr, sizeof(&servreAddr));
    servreAddr.sin_family = AF_INET;
    servreAddr.sin_port = htons(port);
    servreAddr.sin_addr.s_addr = inet_addr(ip.c_str());

    if(::connect(m_fd, (struct sockaddr*)&servreAddr, sizeof(servreAddr)) <0)
    {
        LOG_WARN << "Connect to "<<ip<<":"<<port<<" failed! Cause: "<<strerror(errno)<<ENDL;
        return false;
    }
    return true;
}

uint32_t Socket::send(const char* buff, const int &size)
{
    if(size == 0)
        return ::send(m_fd, buff, strlen(buff), 0);
    return ::send(m_fd, buff, size, 0);
}

int Socket::recv(std::string& buff)
{
    uint32_t ret = ::recv(m_fd, recvBuffer, SOCK_BUFF_SIE,0);
    buff = recvBuffer;
    bzero(recvBuffer, SOCK_BUFF_SIE);
    return ret;
}

bool Socket::close()
{
    return (shutdown(m_fd, SHUT_WR)==0);
}

void Socket::setNonblock()
{
    int flag = fcntl(m_fd, F_GETFL, 0);
    if(-1==flag)
    {
        LOG_WARN << "Get fd status fail! " << m_fd << ENDL; 
        return;
    }
    flag |= O_NONBLOCK;
    int s = fcntl(m_fd, F_SETFL, flag);
    if(-1==s)
        LOG_WARN << "Set fd status fail! " << m_fd << ENDL; 
}

void Socket::setReuse()
{
    int on = 1;
    setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
}