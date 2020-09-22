#pragma once
#include <string>
#include <stdint.h>
#include <netinet/in.h>
#include <memory>

const int SOCK_BUFF_SIE = 4096;

class Socket
{
private:
    /* data */
    const int m_fd;
    char recvBuffer[SOCK_BUFF_SIE];
public:
    Socket(const int &fd);
    ~Socket();

    bool bind(const std::string &ip, const int &flag=INADDR_ANY);
    bool listen(const int maxConn);
    std::shared_ptr<Socket> accept(std::string& addr);
    bool connect(const std::string &ip, const int &port);
    uint32_t send(const char* buff, const int &size=0);
    int recv(std::string& buff);
    bool close();
    void setNonblock();
    void setReuse();
    int fd(){return m_fd;}

    //bool operator == (const Socket& s);

    //TODO:
    //readv 
};


