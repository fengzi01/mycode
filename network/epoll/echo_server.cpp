#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_PORT (8081)
#define SERVER_BACKLOG (5)
#define EPOLL_MAX_SIZE 24 
#define BUFSIZE 1024

int main(int argc, char *argv[])
{
    int listen_fd = 0;
    int client_fd = 0;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    socklen_t client_len;

    int epfd = 0;
    struct epoll_event event, *my_events;

    int active_fds_cnt;
    int i;

    char buf[BUFSIZE];

    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVER_PORT);

    listen_fd = socket(AF_INET,SOCK_STREAM,0);
    bind(listen_fd,(struct sockaddr *)&server_addr,sizeof(server_addr));
    listen(listen_fd,SERVER_BACKLOG);
    
    if (epfd = epoll_create(EPOLL_MAX_SIZE) < 0) {
        perror("epoll create");
        goto END;
    }

    event.events = EPOLLIN;
    event.data.fd = listen_fd;
    if (epoll_ctl(epfd,EPOLL_CTL_ADD,listen_fd,&event) < 0) {
        // 添加监听fd
        perror("epoll ctl add listen_fd");
        goto END;
    }

    printf("Listen on:%d\n",SERVER_PORT);

    my_events = (struct epoll_event *)malloc(sizeof(struct epoll_event) * EPOLL_MAX_SIZE);

    for (;;) {
        active_fds_cnt = epoll_wait(epfd,my_events,EPOLL_MAX_SIZE, -1);
        for (i = 0;i<active_fds_cnt;i++) {
            if (my_events[i].data.fd == listen_fd) {
                // accept
                if ((client_fd = accept(listen_fd,(struct sockaddr*)&client_addr,&client_len)) < 0) {
                    perror("accept");
                    continue;
                }

                char ip[20];
                // 新建连接
                printf("new connection:[%s:%d]\n",inet_ntop(AF_INET,&client_addr.sin_addr,ip,sizeof(ip)),ntohs(client_addr.sin_port));

                // 加入epoll
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = client_fd;
                if (epoll_ctl(epfd,EPOLL_CTL_ADD,client_fd,&event) < 0) {
                    perror("epoll ctl add client_fd");
                    goto END;
                }
            } else if (my_events[i].events & EPOLLIN) {
                printf("EPOLLIN\n");

                client_fd = my_events[i].data.fd;
                int n = read(client_fd,buf,BUFSIZE);
                if (n < 0) {
                    perror("read");
                    continue;
                } else if (n == 0) {
                    printf("read n=0\n");
                    continue;
                    epoll_ctl(epfd,EPOLL_CTL_DEL,client_fd,&event);
                    close(client_fd);
                } else {
                    buf[n] = '\0';
                    printf("read from client:[%s]\n",buf);

                    //write(client_fd,buf,strlen(buf));
                    //printf("write to client:[%s]\n",buf);
                    memset(buf,0,BUFSIZE);
                }
            } else if (my_events[i].events & EPOLLOUT) {
                // 可写
                printf("EPOLLOUT\n");
            }
        }
    }


END:
    close(epfd);
    close(listen_fd);

    free(my_events);
    my_events = NULL;

    return 0;
}
