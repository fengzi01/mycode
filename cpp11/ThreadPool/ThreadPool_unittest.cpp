#include "ThreadPool.h"
static void MySleep(int seconds) {
    timespec t = { seconds, 0 };
    ::nanosleep(&t, NULL);
} 

int main(int argc, char *argv[])
{
    std1::ThreadPool pool(2);
    pool.enqueue(
        []{
            for(int i=0;i<10;++i) {
                fprintf(stderr,"task 1\n");
                MySleep(1);   
            }
        });
    pool.enqueue(
        []{
            for(;;) {
                fprintf(stderr,"task 2\n");
                MySleep(1);   
            }
        });

    std::condition_variable condition;
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    condition.wait(lock);
    return 0;
}
