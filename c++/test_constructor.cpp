#include<iostream>
using namespace std;
//c++中六种默认的构造函数
class Test
{
public:
    Test(int d = 0):m_data(d)//1构造函数(带默认值0)，以参数列表的形式初始化
    {
        cout<<"Creat Test Obj :"<<this<<endl;
    }
    ~Test()//2析构函数
    {
        cout<<"Free Test Obj :"<<this<<endl;
    }
    Test(const Test &t)//3拷贝构造函数：以对象初始化对象
    {
        cout<<"Copy Test Obj :"<<this<<endl;
        m_data = t.m_data;
    }
    Test& operator=(const Test &t)//4赋值语句
    {
        cout<<"Assgin:"<<this<<":"<<&t<<endl;
        if(this != &t)
        {
            m_data = t.m_data;
        }
        return *this;
    }
    Test* operator&()//5对象取址
    {
        return this;
    }
    const Test* operator&()const//6常对象取址
    {
        return this;
    }
    int GetData()
    {
        return m_data;
    }
private:
    int m_data;
};
