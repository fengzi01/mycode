#include <ev.h>
#include <stdio.h>

static void stdin_callback(EV_P_ ev_io *w,int revents) {
    printf("stdin ready\n");
    ev_io_stop(EV_A_ w); // ev_io_stop(loop,w);

    ev_break(EV_A_ EVBREAK_ALL);
}

static void timeout_callback(EV_P_ ev_timer *w,int revents) {
    printf("timeout\n");
    ev_break(EV_A_ EVBREAK_ONE);
}

int main(int argc, char *argv[])
{
    struct ev_loop *loop = EV_DEFAULT; // reactor

    ev_io stdin_watcher;
    ev_timer timeout_watcher; // event handler

    ev_io_init(&stdin_watcher,stdin_callback,0,EV_READ); // 初始化事件处理器
    ev_io_start(loop,&stdin_watcher); // 注册服务

    ev_timer_init(&timeout_watcher,timeout_callback,5.5,0);
    ev_timer_start(loop,&timeout_watcher);

    ev_run(loop,0); // run event_loop

    return 0;
}
