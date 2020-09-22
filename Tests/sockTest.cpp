#include "Reactor/Socket/Socket.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include "Log/Logging.h"
#include <iostream>
#include "Log/AsynLog.h"

using namespace std;

int main()
{
    int sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    Socket s(sockfd);
    if (!s.connect("192.168.0.6", 8080))
        return 1;
    for(int i=0; i<10; i++)
    {
        s.send("Send to server message!");
    }
    LOG_INFO << "AAAAAAAAAAAAAAAA"<<ENDL;
    std::string recvStr;
    //for(int i=0; i<3; i++)
    while(1)
    {
        int ret = s.recv(recvStr);
        if(!ret) return 0;
        LOG_INFO << recvStr.c_str() <<ENDL;
        cout<<"Recv "<<recvStr<<endl;
        recvStr.clear();
    }

    return 0;
}