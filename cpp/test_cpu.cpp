#include <iostream>
using namespace std;

void checkCPU()
{
    union MyUnion{
        int a;
        short c;
    }test;
    test.a = 1;
    if (test.c == 1)
        cout << "little endian" <<endl;
    else cout << "big endian" <<endl;
}

int main()
{
    checkCPU();
    return 0;
}

