#include "bitmap.h"

int main(int argc, char *argv[])
{
    mycode::bitmap bitmap;
    int num = 300;
    uint64_t max_tid = 5472508508;
    // uint64_t max_tid = 500;
    printf("max_tid:%lu\n",max_tid);

    bitmap.create(max_tid);
    bitmap.set(max_tid);
    bool flag = bitmap.test(max_tid);
    printf("set_and_test:%d\n",flag);

    bitmap.unset(max_tid);
    flag = bitmap.test(max_tid);
    printf("unset_and_test:%d\n",flag);

    uint64_t tid = 499;
    bitmap.set(tid);
    flag = bitmap.test(tid);
    printf("set_and_test:%d\n",flag);


    bitmap.destroy();

    return 0;
}
