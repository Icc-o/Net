//用来测试带外数据的客户端

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

using namespace std;

int main()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    bzero(&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8989);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
    
    int ret = connect(sock_fd, (struct sockaddr*)&serverAddr,  sizeof(serverAddr));
    assert(ret != -1);

    char buff[] = "This is a normal message to server!";
    write(sock_fd, buff, strlen(buff));

    send(sock_fd, buff, strlen(buff), MSG_OOB);
    send(sock_fd, buff, 1, MSG_OOB);

    return 0;
}