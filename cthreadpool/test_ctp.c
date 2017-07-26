#include "cthread_pool.h"
#include "stdio.h"
#include <unistd.h>  

void* task_routine(long no) {
    unsigned int i;
    for (i=0;i<5;i++) {
        printf("tid:%ld task:%ld\n", (long)pthread_self(), no);  
        fflush(stdout);  
        sleep(2);  
    }
    return NULL;
}

int main() {
    long i;
    cthread_pool_t pool;
    
    pool = ctp_create(2);
     
    for (i=0;i<5;i++) {
        ctp_add_task(pool,(void*(*)(void*))task_routine,(void*)i);
    }

    puts("press enter to terminate ...");  
    getchar();  
    ctp_destroy(pool);

    return 0;
}
