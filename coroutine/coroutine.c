#include <stdlib.h> // malloc
#include <stdio.h>
#include "coroutine.h"

typedef struct {
    co_t coroutines[MAX_CO_SIZE];
    int64_t max_co_idx;
    int64_t running_co_idx;
    ucontext_t main_uctx;
} co_sched_t;

co_sched_t *g_sched = NULL;

static void create_g_sched() {
    if (g_sched) {
        return;
    }
    co_sched_t *sched = (co_sched_t *)malloc(sizeof(co_sched_t));
    if (sched == NULL) {
        perror("malloc co_sched_t");
    }

    sched->max_co_idx = 0;
    sched->running_co_idx = -1;
    for (int i = 0; i < MAX_CO_SIZE; ++i) {
        sched->coroutines[i].id = i;
        sched->coroutines[i].state = CO_STATE_FREE;
    }

    g_sched = sched;
}

static co_sched_t * get_or_new_sched() {
    if (!g_sched) {
        create_g_sched();
    }
    return g_sched;
}

static void run_this(co_sched_t *sched) {
    int64_t run_idx = sched->running_co_idx;
    if (run_idx >= 0) {
        co_t *co = &(sched->coroutines[run_idx]);
        co->fn(co->fnarg);
        co->state = CO_STATE_FREE;
        sched->running_co_idx = -1;
    }
}

int co_create(co_t **c, void *(*fn)(void *), void *arg) {
    co_sched_t * sched = get_or_new_sched();

    int i = 0;
    for (; i < MAX_CO_SIZE; ++i) {
        if (sched->coroutines[i].state == CO_STATE_FREE) {
            break;
        }
    }

    if (i >= MAX_CO_SIZE) {
        return -1;
    }
    
    co_t *co = &(sched->coroutines[i]);
    *c = co;

    co->state = CO_STATE_RUNNABLE;
    co->fn = fn;
    co->fnarg = arg;

    getcontext(&(co->uctx));

    co->uctx.uc_stack.ss_sp = co->ustack;
    co->uctx.uc_stack.ss_size = CO_STACK_SIZE;
    co->uctx.uc_stack.ss_flags = 0;
    co->uctx.uc_link = &(sched->main_uctx);

    makecontext(&(co->uctx),(void(*)(void))run_this,1,sched);

    sched->running_co_idx = co->id;
    // run coroutine
    swapcontext(&(sched->main_uctx), &(co->uctx));

    return 0;
}

void co_yield() {
    co_sched_t *sched = get_or_new_sched();
    int64_t running_idx = sched->running_co_idx;
    if (running_idx >= 0) {
        co_t *co = &(sched->coroutines[running_idx]);
        co->state = CO_STATE_SUSPEND;
        sched->running_co_idx = -1;
        swapcontext(&(co->uctx), &(sched->main_uctx));
    }
}

void co_resume(co_t *co) {
    co_sched_t *sched = get_or_new_sched();
    if (sched->running_co_idx >= 0) {
        fprintf(stderr,"resume another coroutine in a coroutine!\n");
        return;
    }

    if (co->state == CO_STATE_SUSPEND) {
        sched->running_co_idx = co->id;
        co->state = CO_STATE_RUNNING;
        swapcontext(&(sched->main_uctx), &(co->uctx));
    }
}

int co_is_alive(const co_t *co) {
    (void)co;
    fprintf(stderr, "not implement yet\n");
    return 0;
}
