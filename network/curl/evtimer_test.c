// 利用libev库实现定时打印值

#include <ev.h>
#include <stdio.h>
#define MSG_OUT stdout

static void timer_cb(EV_P_ struct ev_timer *w, int revents){
    fprintf(MSG_OUT,"timer out:%d\n",revents);
    static int cb_count = 0;  
    fprintf(MSG_OUT,"timer_cb() call, cb_count = %d\n", cb_count++); 
}
int main(int argc, char *argv[])
{
    struct ev_loop *loop = ev_default_loop(0);
    struct ev_timer timer_ehr; // event handler

    ev_timer_init(&timer_ehr,timer_cb,1.0,2);
    ev_timer_start(loop,&timer_ehr);

    ev_run(loop,0);
    return 0;
}
