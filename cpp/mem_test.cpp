#include <cstdio>
#include <cstdlib>

using namespace std;

typedef void (*FP)();
FP fp;

void fun() {
    int t;
    printf("〇〇〇栈区：0x%08X\n", (unsigned)&t);
}

class Base {
    public:
    virtual void gao() {
        printf("Base\n");
    }
    virtual ~Base() {}
};

class Derived : Base {
public:
    virtual void gao() {
        printf("Derived\n");
    }
    virtual void dio() {
        printf("URYYYYYY\n");
    }
    virtual ~Derived() {}

};

Base b;
Derived d;

int glob = 0;
int glob2 = 0;

const int cons = 0;

int main()
{
    int stk = 0;
    int stk2 = 0;
    const char *p = "123456";
    const char *p2 = "3333";
    unsigned table = *((unsigned*)&b);
    unsigned table2 = *((unsigned*)&d);

    printf("sizeof(void*) %d\n", sizeof(void*));

    char *heap =(char *) malloc(100);
    char *heap2 = new char[100];

    printf("〇〇〇栈区：0x%08X\n", (unsigned)&stk);
    printf("〇〇〇栈区：0x%08X\n", (unsigned)&stk2);
    fun();

    fp = fun;
    printf("〇〇函数区：0x%08X\n", (unsigned)&(*fp));

    printf("〇〇常量区：0x%08X\n", (unsigned)&(*p));
    printf("〇〇常量区：0x%08X\n", (unsigned)&(*p2));
    printf("〇〇常量区：0x%08X\n", (unsigned)&cons);

    printf("〇虚函数表：0x%08X\n", table);
    printf("〇虚函数表：0x%08X\n", table2);

    printf("全局静态区：0x%08X\n", (unsigned)&glob);
    printf("全局静态区：0x%08X\n", (unsigned)&glob2);

    printf("〇〇〇堆区：0x%08X\n", (unsigned)&(*heap));
    printf("〇〇〇堆区：0x%08X\n", (unsigned)&(*heap2));

    return 0;
}
