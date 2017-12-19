#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>

#define SERVER_PORT (8080)
#define SERVER_BACKLOG (5)
#define BUFFER_SIZE (4096)

int main(int argc, char *argv[])
{
    int listen_fd = 0,client_fd = 0;

    struct sockaddr_in listen_addr;
    struct sockaddr_in remote_addr;

    unsigned int sin_size = 0;

    char buff[BUFFER_SIZE];

    // 初始化listen地址
    memset(&listen_addr,0,sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = INADDR_ANY;
    listen_addr.sin_port = htons(SERVER_PORT);

    // 创建socket
    // int socket(int domain, int type, int protocol);
    if ((listen_fd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("Socket Error:");
        return -1;
    }

    // 绑定socket
    if (bind(listen_fd,(struct sockaddr *)&listen_addr,sizeof(struct sockaddr)) < 0) {
        perror("Bind Error:");
        return -2;
    }

    // 监听
    if (listen(listen_fd,SERVER_BACKLOG) < 0) {
        perror("Listen Error:");
        return -3;
    }
    
    printf("Listen on:%d\n",SERVER_PORT);
    
    // 循环
    for (;;) {
        if ((client_fd = accept(listen_fd,(struct sockaddr *)&remote_addr,&sin_size)) < 0) {
            perror("Accept Error:");
            exit(-1);
        }

        int n = recv(client_fd,buff,BUFFER_SIZE,0);
        buff[n] = '\0';
        
        printf("recv msg from client:%s\n",buff);
        // 发送数据
        if (send(client_fd,buff,n,0) != n) {
            perror("Write Error:");
            exit(-1);
        }

        close(client_fd);
    }

    return 0;
}
