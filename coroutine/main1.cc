#include <stdio.h>
#include <stdlib.h>
#include "coroutine.h"

int product;

void *producer(void *arg) {
    printf("fn1 fn1 fn1\n");
    for (int i=0;i<5;i++) {
        printf("produce i=%d\n",i);
        product=i;
        co_yield();
    }
}

void *consumer(void *arg) {
    printf("fn2 fn2 fn2\n");
    for (int i=0;i<5;i++) {
        printf("fn2 fn2 fn2 after product = %d\n", product);
        co_yield();
    }
}

int main() {
    co_t *co1;
    co_t *co2;
    printf("main main main \n");
    co_create(&co1,producer,NULL);
    co_create(&co2,consumer,NULL);
    printf("main main main after\n");
    while (co1->state != CO_STATE_FREE && co2->state != CO_STATE_FREE) {
        co_resume(co1);
        co_resume(co2);
    }
}
