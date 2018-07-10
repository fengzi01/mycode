#include <iostream>
#include <ucontext.h>
using namespace std;

static char g_stack[2048];
static ucontext_t ctx_func,ctx_main;

void func()
{
    // do something.
    cout << "enter func" << endl;

    int a = 10;

    swapcontext(&ctx_func, &ctx_main);

    cout << "func1 resume from yield, a = " << a << endl;
    // continue to do something.
}

int main()
{
    // 获取上下文
   getcontext(&ctx_func);
   ctx_func.uc_stack.ss_sp = g_stack;
   ctx_func.uc_stack.ss_size = sizeof g_stack;
   
   // 返回main函数中
   ctx_func.uc_link = &ctx_main;
    
   // 生成上下文
   makecontext(&ctx_func, func, 0);

   cout << "in main, before coroutine starts" << endl;

   // 保存上下文，并激活ctx_func的上下文
   swapcontext(&ctx_main, &ctx_func);

   cout << "back to main" << endl;

   swapcontext(&ctx_main, &ctx_func);
   
   cout << "back to main again" << endl;
   return 0;
}
