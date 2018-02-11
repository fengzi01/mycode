#include <ev.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#define SERVER_PORT 8082
#define BUF_SIZE 1024

#define  PEER_IP_LENGTH      16

static int getpeerinfo(int fd, char* ipaddr, int *port)
{
	struct sockaddr_in name;
	socklen_t namelen = sizeof(name);
	struct hostent* result;

	/*
	 * Clear the memory.
	 */
	memset(ipaddr, '\0', PEER_IP_LENGTH);
	if (::getpeername(fd, (struct sockaddr *)&name, &namelen) != 0) {
		return -1;
	} 
	strncpy(ipaddr,
			inet_ntoa(*(struct in_addr *)&name.sin_addr.s_addr),
			PEER_IP_LENGTH);
	*port = ntohs(name.sin_port);
  }

static void client_read_cb(struct ev_loop *loop,ev_io *eh /*event handler*/,int revents /*event type*/) {
    char buf[BUF_SIZE];
    if (EV_ERROR & revents) {
        perror("error event in read");
        return;
    }

    int n = recv(eh->fd,buf,BUF_SIZE,0);
    if (n < 0) {
        perror("read error");
        return;
    } else if (n > 0) {
        buf[n] = '\0';
        printf("recv msg:[%s]\n",buf);
        
        // echo 
        send(eh->fd,buf,n,0);
        bzero(buf,n+1);
    } else {
        // close
        // tcp关闭连接，客户端先发送 fin信号；服务器端发送ack信号确认
		char ip[PEER_IP_LENGTH];
        int port = 0;
        getpeerinfo(eh->fd,ip,&port);

        printf("close connect:[%s:%d]\n",ip,port);

        /* 关闭连接 */
        ev_io_stop(loop,eh);
        close(eh->fd);
        free(eh);
    }
}

static void accept_cb(struct ev_loop *loop,ev_io *eh /*event handler*/,int revents /*event type*/) {
    if (EV_ERROR & revents) {
        perror("error event in accept");
        return;
    }

    int client_fd;
    sockaddr_in client_addr;
    socklen_t sin_len;
    struct ev_io *client_eh;

    char ip[20];
    int port;


    if ((client_fd = accept(eh->fd,(struct sockaddr *)&client_addr,&sin_len)) < 0) {
        perror("accept error");
        return;
    }
    printf("new connection:[%s:%d]\n",inet_ntop(AF_INET,&client_addr.sin_addr,ip,sizeof(ip)),ntohs(client_addr.sin_port));

    // getpeerinfo(client_fd,ip,&port);
    // printf("new connection:[%s:%d]\n",ip,port);
    
    /* 初始化客户端连接event handler */
    client_eh = (struct ev_io *)malloc(sizeof(struct ev_io));
    memset(client_eh,0,sizeof(*client_eh));
    ev_io_init(client_eh,client_read_cb,client_fd,EV_READ);
    ev_io_start(loop,client_eh);
}

int main(int argc, char *argv[])
{
    /* 初始化监听fd */
    int listen_fd;
    struct sockaddr_in listen_addr;

    if ((listen_fd = socket(AF_INET,SOCK_STREAM,0)) < 0) { // 赋值语句返回的是所赋的值
        perror("socket error");
        return -1;
    }

    memset(&listen_addr,0,sizeof(listen_addr));
    listen_addr.sin_family = AF_INET;
    listen_addr.sin_port = htons(SERVER_PORT);
    listen_addr.sin_addr.s_addr = INADDR_ANY;

    if ((bind(listen_fd,(struct sockaddr*)&listen_addr,sizeof(listen_addr))) != 0) {
        perror("bind error");
        return -1;
    }

    if (listen(listen_fd,5) < 0) {
        perror("listen error");
        return -1;
    }

    /* 初始化ev */
    struct ev_loop *loop = EV_DEFAULT; // reactor
    struct ev_io listen_eh; // event_handler

    ev_io_init(&listen_eh,accept_cb,listen_fd,EV_READ); // listen_fd:handle;accept_cb:handle_event
    ev_io_start(EV_A_ &listen_eh); // regist_handler

    /* 执行event_loop */
    ev_run(EV_A_ 0); // demultiplexer

    return 0;
}
