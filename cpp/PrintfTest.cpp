#include "stdio.h"
#include <string.h>
#include <stddef.h>

int main(int argc, char *argv[])
{
    char buf[32];
    double b = 0.12345;
    memset(buf,0,sizeof buf);
    size_t len = ::snprintf(buf, 32, "%.12g", b);   
    printf("len:%lu,data:%s\n",len,buf);
    return 0;
}
