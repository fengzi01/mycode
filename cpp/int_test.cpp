#include <stdio.h>

int main(int argc, char *argv[])
{
//    int month = 09; // 报错
    int day1 = 07;
    int day2 = 7;

    long double ld = 3.1415926122;
//    int a = {ld}; // 防止丢失精度的初始化

    int a = 10;
    int *pa = &a;
    int &ra = a;

    pa = &ra;

    printf("%d\n",*pa);

    float val = 10.0;
    const int &cr = val;
    // int &cr = val; 非法
    
    const int & cr1 = 10;

    const int * const cp1 = &a;

    return 0;
}
