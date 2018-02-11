#include <sys/socket.h>
#include <event.h>
#include <vector>

int tcp_server_init(int port,int backlog) {
}

int main(int argc, char *argv[])
{
    int server_fd;
    server_fd = socket(AF_INET,SOCK_STREAM,0);

    event_base_new();
    std::vector<int> v;
    
    return 0;
}
