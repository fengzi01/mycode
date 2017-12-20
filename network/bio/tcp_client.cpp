#include <netinet/in.h>
#include <arpa/inet.h> // inet_addr
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFSIZE (4096)
#define REMOTE_IP "127.0.0.1"
#define REMOTE_PORT 8081

int main(int argc, char *argv[])
{
    int client_fd;

    char *port = argv[1];

    struct sockaddr_in remote_addr;
    char buff[BUFSIZE];
    char data[] = "Hello server!";

    memset(&remote_addr,0,sizeof(remote_addr));
    remote_addr.sin_family = AF_INET;
    remote_addr.sin_addr.s_addr = inet_addr(REMOTE_IP);
    //remote_addr.sin_port = htons(REMOTE_PORT);
    remote_addr.sin_port = htons(atoi(port));

    // socket
    if ((client_fd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
        perror("Socket Error");
        return -1;
    }

    // connect()时由系统随机生成一个IP和port
    if (connect(client_fd,(struct sockaddr *)&remote_addr,sizeof(remote_addr)) < 0) {
        perror("Connect Error:");
        return -2;
    }

    printf("Connected to server\n");

    if (send(client_fd,data,strlen(data),0) < 0) {
        perror("Send Error:");
        return -3;
    }

    int n = 0;
    if ((n = recv(client_fd,buff,BUFSIZE,0)) < 0) {
        perror("Recv Error:");
        return -4;
    }

    buff[n] = '\0';
    printf("Recv data from server:%s\n",buff);
    close(client_fd);

    return 0;
}
