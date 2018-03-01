#include "Thread.h" 
static void mysleep(int seconds)
{
    timespec t = { seconds, 0 };
    nanosleep(&t, NULL);
}

static void func1(int i) {
    for(;;) {
        printf("thread func:%d\n",i);
        mysleep(1);
    }
}

int main(int argc, char *argv[])
{
    std1::Thread thread1(std::bind(func1,1));
    std1::Thread thread2(std::bind(func1,2));
    std1::Thread thread3(std::bind(func1,3));
    thread1.join();
    thread2.join();
    thread3.join();
    return 0;
}
