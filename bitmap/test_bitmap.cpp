#include "bitmap.h"

int main(int argc, char *argv[])
{
    mycode::bitmap bitmap;
    int num = 300;
    bitmap.create(num);
    bitmap.set(num);
    bool flag = bitmap.test(num);
    printf("set_and_test:%d\n",flag);

    bitmap.unset(num);
    flag = bitmap.test(num);
    printf("unset_and_test:%d\n",flag);

    return 0;
}
