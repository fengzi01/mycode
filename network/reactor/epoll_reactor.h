#ifndef _EPOLL_REACTOR
#define _EPOLL_REACTOR 
#include <sys/epoll.h>
typedef struct _epr_event {
    int fd;
    void (*call_back)(int fd,int events);
    unsigned long long last_active;
} epr_event_t;
#endif /* ifndef _EPOLL_REACTOR */
