#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/types.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <vector>

using namespace std;

const int MAX_CONNECTION = 1000;

int main()
{

    //==========创建server fd并监听========================
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);

    int reuse = 1;
    int ret = setsockopt(sockfd,SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
    assert(ret != -1);

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    serverAddr.sin_port = htons(8989);

    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    assert(ret!=-1);

    ret = listen(sockfd, MAX_CONNECTION);
    assert(ret!=-1);
    //======================================================

    fd_set readfds;
    fd_set exceptfds;

    vector<int> connected_fds;      //所有链接上来的fd

    int retval, maxfd=sockfd;
    char recv_buf[4096];
    while(1)
    {
        FD_ZERO(&readfds);
        FD_ZERO(&exceptfds);
        
        FD_SET(sockfd, &readfds);
        maxfd = sockfd;
        for(int i=0; i<connected_fds.size(); i++)
        {
            FD_SET(connected_fds[i], &readfds);
            FD_SET(connected_fds[i], &exceptfds);
            if(connected_fds[i] > maxfd) 
                maxfd = connected_fds[i];   
        }
        retval = select(maxfd+1, &readfds, nullptr, &exceptfds, nullptr);
        assert(retval != -1);

        //新的连接
        if(FD_ISSET(sockfd, &readfds))
        {
            int client_fd = accept(sockfd,nullptr, nullptr);
            connected_fds.push_back(client_fd);
            cout<<"New client "<<client_fd<<" connected!"<<endl;
            continue;
        }
        else
        {
            //连接的端口接收到消息
            for(int i=0; i<connected_fds.size(); i++)
            {
                if(FD_ISSET(connected_fds[i], &readfds))
                {
                    int nbytes = read(connected_fds[i], recv_buf, 4096);
                    //断开连接
                    if(nbytes == 0)
                    {
                        cout<<"Client "<<connected_fds[i]<<" disconnected"<<endl;
                        close(connected_fds[i]);
                        connected_fds.erase(connected_fds.begin()+i);
                        continue;
                    }else{
                        cout<<"Recv buf: "<<recv_buf<<endl;
                        memset(recv_buf, 0, 4096);
                    }
                }
                else if(FD_ISSET(connected_fds[i], &exceptfds))
                {
                    cout<<"Got except fd!"<<connected_fds[i]<<endl;
                    //接收带外数据
                    int nbytes = recv(connected_fds[i], recv_buf, 4096, MSG_OOB);
                    if(nbytes <= 0)
                        cout<<"Got no OOB data"<<endl;
                    else
                    {
                        cout<<"Got OOB data: "<<recv_buf<<endl;
                        memset(recv_buf, 0, 4096);
                    }
                    
                }
            }
        }
    }
    return 0;
}