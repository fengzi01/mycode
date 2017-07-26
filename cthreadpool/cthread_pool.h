// cthread_pool.h
// c实现的线程池

#ifndef CTHREAD_POOL_H_INCLUDED
#define CTHREAD_POOL_H_INCLUDED

#include "pthread.h"
#include "cqueue.h"

typedef struct cthread_pool* cthread_pool_t;

struct cthread_pool {
    // 线程数目
    unsigned int thread_count;
    // 线程id列表
    pthread_t *thread_list;
    cqueue_t task_queue;
    pthread_mutex_t mutex_lock;
    pthread_cond_t task_ready;
};

typedef struct ctask {
    void* (*routine)(void *arg);
    void* arg;
} *ctask_t;

void mutex_lock_cleanup(pthread_mutex_t *lock);

// 创建线程池
cthread_pool_t ctp_create(unsigned int count);

// 向线程池中添加任务
int ctp_add_task(cthread_pool_t p,void* (*routine)(void *arg),void *arg);

// 销毁线程池
void ctp_destroy(cthread_pool_t p);

#endif // CTHREAD_POOL_H_INCLUDED
