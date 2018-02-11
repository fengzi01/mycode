#include "stddef.h"
#include "stdio.h"
#include "string.h"

int main(int argc, char *argv[])
{
    printf("%ld\n",sizeof(size_t));
    printf("%ld\n",sizeof(int));

    int a[100];
    int b[100];

    memcpy(a,b,-1);
    return 0;
}
