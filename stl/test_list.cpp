/**
 * stl实现的双向链表
 */
#include <list>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    std::list<int> lst1;
    list<int> lst2(2);
    list<int> lst3(2,2); // 初始化为2
    for (int i : lst3) {
        cout << i << endl;
    }

    list<int> lst4(lst3);
    list<int> lst5(lst4.begin(),lst4.end());

    lst5.assign(lst4.begin(),lst4.end());
    lst5.push_back(5);
    lst5.pop_back(); // 删除尾值
    lst5.clear();

    bool isEmpty = lst5.empty();
    lst5.erase(lst5.begin(),lst5.end());
    lst5.front();
    lst5.back();

    lst5.insert(lst5.begin(),3);
    lst5.insert(lst5.begin(),3,2);
    
    lst5.remove(5);
    lst5.reverse();

    lst5.unique();
    lst5.sort();
    lst5.size();

    lst5.rbegin(); // 干嘛的？

    for (auto iter = lst5.begin();iter != lst5.end();iter++) {
        cout << *iter << " ";
    }
    cout << endl;
    return 0;
}
