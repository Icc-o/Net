/**
 * 为了查看TCP的慢启动过程的客户端程序
 * 发送n个大小为m的数据报到服务端
 * 使用tcpdump抓包，逐个包分析
 * 
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <assert.h>
#include <string.h>
#include <iostream>
#include <stdio.h>

int main(int argc, char** argv)
{
    assert(argc == 3)
    const int times = atoi(argv[1]);
    const int length = atoi(argv[2]);

    inf fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(fd>0);

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(struct sockaddr_in));
    server_addr.sin_family = AD_INET;
    server_addr.sin_addr.s_addr = inet_addr("192.168.6.218");
    server_addr.sin_port = htons(8899);

    int ret = connect(fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr));
    assert(ret==0)

    char send_buffer[length];
    for(int i=0; i<length; i++)
        send_buffer[i] = 'A';
    
    for(int i=0; i<times; i++)
    {
        size_t size = send(fd, send_buffer, length);
        assert(size != -1);
    }

    close(fd);
    return 0;
}
