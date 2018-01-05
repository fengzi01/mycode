// 模板编程测试
#include<stdio.h>

template<class T>
T _max(const T &a,const T &b) {
    return a > b ? a : b;
}

int main(int argc,char **argv) {
    int a = 4;
    int b = 3;

    printf("max:%d\n",_max(a,b));
}
