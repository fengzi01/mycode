#include <iostream>
using namespace std;
class CExample 
{
private:
 int a;

public:
 //构造函数
 CExample(int b)
 { 
  a = b;
 }

 //拷贝构造
 CExample(const CExample& C)
 {
  a = C.a;
  cout<<"copy"<<endl;
 }

     void Show ()
     {
         cout<<a<<endl;
     }
};

//全局函数
CExample g_Fun()
{
 CExample temp(0);
 return temp;
}

int main()
{
 g_Fun();
 return 0;
}
