#include "Reactor/EventLoop/EventLoop.h"
#include "Reactor/Socket/TcpServer.h"
#include "Reactor/Epoll/EpollPoller.h"
#include "Util/Util.h"
#include "Log/Logging.h"
#include "HttpReq.h"
#include <iostream>
using namespace std;

void recvCb(std::shared_ptr<Socket> sock, const char* buff)
{
    LOG_INFO << "recv message: "<< buff <<" from "<<sock->fd() <<ENDL;
}

void httpStrTest(std::shared_ptr<Socket> sock, const char* buff)
{
   HttpReq req;
   req.parseReq(buff);
   LOG_INFO <<"[Connection]: " << req.getHeader("Connection") <<ENDL;
   LOG_INFO <<"[Host]: " << req.getHeader("Host") <<ENDL;
}

int main(int argc, char **argv)
{
    cout<< "argv[1]" <<argv[1]<<endl;
    if(strcmp(argv[1], "server")==0)
    {
        cout<<"server!"<<endl;
        Logger::setLogName("TcpServer");
        
        EventLoop loop;
        TcpServer server("127.0.0.1", 8080, &loop);
        server.SetRecvCb(recvCb);
        server.start();

        while(1)
        {
            mSleep(1000);
        }
    }
    else
    {
        Logger::setLogName("TcpClient");
        int nCount = atoi(argv[2]);
        cout<<"count: "<<nCount<<endl;
        std::vector<std::shared_ptr<Socket>> clients;
        for(int i=0;i <nCount; i++)
        {
            int fd = socket(AF_INET, SOCK_STREAM, 0);
            std::shared_ptr<Socket> s = std::make_shared<Socket>(fd);
            if(s->connect("192.168.0.6", 8080))
            {
                s->setNonblock();
                clients.push_back(s);
                LOG_INFO<< "Client "<<fd<<ENDL;
            }
            //mSleep(10);
        }
        int i=0;
        while(i<100000)
        {
            for(auto &c : clients)
                c->send("Pressure test");
            mSleep(10);
            ++i;
        }
    }

    return 0;
}