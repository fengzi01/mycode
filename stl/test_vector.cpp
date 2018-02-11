#include<vector>
#include<iostream>
// 顺序容器
// 基于连续内存
using namespace std;

int main(int argc, char *argv[])
{
    vector<int> vec1; // 默认构造函数
    vector<int> vec2(vec1);
    vector<int> vec3(vec1.begin(),vec1.end());
    vector<int> vec4(10);
    vector<int> vec5(10,4);
    vector<vector<int> > vec6(10,vector<int>(10));

    vec1.push_back(1); // 添加元素
    int size = vec1.size();
    bool isEmpty = vec1.empty();

    cout << vec1[0] << endl;
    vec6[1][1] = 10;
    cout << vec6[1][1] << endl;

    vector<int>::iterator iter = vec1.begin();
    vector<int>::const_iterator c_iter = vec1.begin();

    size_t len = vec1.size();
    for (size_t i=0;i<len;i++) {
        cout << vec1[i];
    }
    cout << endl;
    cout << "size_t:" << sizeof(size_t) << endl;

    for (;c_iter != vec1.end();c_iter++) {
        cout << *c_iter;
    }
    cout << endl;
    return 0;
}
