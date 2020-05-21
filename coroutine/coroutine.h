#ifndef COROUTINE_H
#define COROUTINE_H 

#ifdef __cplusplus
extern "C" {
#endif

#if __APPLE__ && __MACH__
	#include <sys/ucontext.h>
#else 
	#include <ucontext.h>
#endif

#include <stdint.h>

#define CO_STACK_SIZE (128*1024)
#define MAX_CO_SIZE 1024

#define CO_STATE_FREE 1
#define CO_STATE_RUNNABLE 2
#define CO_STATE_RUNNING 3
#define CO_STATE_SUSPEND 4

typedef struct {
    int64_t id;
    ucontext_t uctx;
    void *(*fn)(void *);
    void *fnarg;
    int8_t state;
    int8_t ustack[CO_STACK_SIZE];
}co_t;

int co_create(co_t **co, void *(*fn)(void *), void *arg);
void co_resume(co_t *co);
void co_yield();
int co_is_alive(const co_t *co);

#ifdef __cplusplus
}
#endif /* end of __cplusplus */

#endif /* ifndef COROUTINE_H */
