#include <stdio.h>
#include <stdlib.h>
#include "coroutine.h"

void *fn1(void *arg) {
    printf("fn1 fn1 fn1\n");
    co_yield();
    printf("fn1 fn1 fn1 after\n");
}

void *fn2(void *arg) {
    printf("fn2 fn2 fn2\n");
    int a = 10;
    co_yield();
    printf("fn2 fn2 fn2 after a = %d\n", a);
}

int main() {
    co_t *co1;
    co_t *co2;
    printf("main main main \n");
    co_create(&co1,fn1,NULL);
    co_create(&co2,fn2,NULL);
    printf("main main main after\n");
    co_resume(co2);
    co_resume(co1);
}
