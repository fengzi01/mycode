#include "cqueue.h"
#include "stdlib.h"
#include "stdio.h"

int main() {
    cqueue_t q = cq_create();

    int i = 0;
    int *data = NULL;

    for(;i<100;i++) {
        data = (int *)cq_enqueue(q,sizeof(int));
        *data = i;
    }
    
    i = 0;
    while (!cq_empty(q)) {
        data = cq_dequeue(q);
        printf("i:%d\tdata:%d\n",++i,*data);
        free(data);
    }

    cq_destroy(q);
}
