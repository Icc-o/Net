#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(listenfd >= 0);
	
	struct sockaddr_in listen_addr;
	memset(& listen_addr, 0, sizeof(sockaddr_in));
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_port = htons(8899);
	listen_addr.sin_addr.s_addr = INADDR_ANY;
	
	int ret = bind(listenfd, (struct sockaddr*)&listen_addr, sizeof(sockaddr));
	assert(ret == 0);
 	
	ret = listen(listenfd, 10);
	assert(ret==0);
	
	socklen_t len;
	int client_fd = accept(listenfd, NULL, &len);
	assert(client_fd >=0 );

    size_t recv_count;
    char message[1024];
    while(1)
    {
        recv_count = recv(client_fd, message, 1024);
        assert(recv_count != -1);

        if(recv_count == 0)
            break;
    }
    close(listenfd);
    close(client_fd);
    return 0;
}