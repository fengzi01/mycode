/**
 * 一个多线程不加锁对同一变量累加的结果
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

int g_count = 0;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

void* thrfun(void *data) {
    (void)data;

    pthread_t self = pthread_self();

    int i = 10000000;
    for (; i>0; --i) {
        pthread_mutex_lock(&g_mutex);
        g_count++;
        pthread_mutex_unlock(&g_mutex);
    }
    return 0;
}

int main(int argc, char *argv[])
{
    int N = 10;
    pthread_t tid[N];
    int i = 0;

    for (;i<N;i++) {
        pthread_create(&tid[i],NULL,thrfun,NULL);
//        sleep(1);
    }

    for(i=0;i<N;i++) {
        pthread_join(tid[i],NULL);
    }

    printf("g_count:%d\n",g_count);
    return 0;
}
