#include "cthread_pool.h"
#include "stdlib.h"
#include "cqueue.h"
#include "pthread.h"

// 工作线程函数
static void* woker_routine(cthread_pool_t p) {
    ctask_t task;
    while(1) {
        pthread_mutex_lock(&p->mutex_lock);
        pthread_cleanup_push((void(*)(void*))mutex_lock_cleanup,&p->mutex_lock);
        while(cq_empty(p->task_queue)) {
            // 当线程被唤醒时，还需要争抢互斥锁啊。
            pthread_cond_wait(&p->task_ready,&p->mutex_lock);
        }
        task = cq_dequeue(p->task_queue);
        pthread_cleanup_pop(0);
        pthread_mutex_unlock(&p->mutex_lock);
        
        // 执行任务函数
        task->routine(task->arg);
        free(task);
    }
}

void mutex_lock_cleanup(pthread_mutex_t *lock) {
    pthread_mutex_unlock(lock);
}

cthread_pool_t ctp_create(unsigned int count) {
    int i = 0;
    cthread_pool_t p = (cthread_pool_t)malloc(sizeof(struct cthread_pool));    
    p->thread_count = count;
    p->thread_list = (pthread_t*)malloc(sizeof(pthread_t)*count);

    // 创建任务队列
    p->task_queue = cq_create();

    // 初始化互斥锁
    pthread_mutex_init(&p->mutex_lock,NULL);
    // 初始化线程条件变量
    pthread_cond_init(&p->task_ready,NULL);

    for (;i<count;i++) {
        // 初始化线程
        // 将线程池指针传递给线程
        int isOk = pthread_create(p->thread_list+i,NULL,(void* (*)(void*))woker_routine,p);
        if (isOk < 0) {
            exit(-1);
        }
    }

    return p;
}

int ctp_add_task(cthread_pool_t p,void* (*routine)(void *arg),void *arg) {
    if (p==NULL) {
        return -1;
    }
    // 加锁
    pthread_mutex_lock(&p->mutex_lock); 
    // 入队列操作
    ctask_t task = cq_enqueue(p->task_queue,sizeof(struct ctask));
    task->routine = routine;
    task->arg = arg;
    // 释放锁
    pthread_mutex_unlock(&p->mutex_lock);
    // 通知工作线程
    // 惊群效应
    pthread_cond_signal(&p->task_ready);

    return 0;
}

// 销毁线程池
void ctp_destroy(cthread_pool_t p) {
    unsigned int i;
    for (i=0;i<p->thread_count;i++) {
        // 销毁线程
        pthread_cancel(*(p->thread_list+i));
    }

    for (i=0;i<p->thread_count;i++) {
        // 销毁线程
        pthread_join(*(p->thread_list+i),NULL);
    }

    pthread_cond_destroy(&p->task_ready);
    pthread_mutex_destroy(&p->mutex_lock);
    cq_destroy(p->task_queue);

    free(p->thread_list);
    free(p);
}

