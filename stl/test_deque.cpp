/**
 * deque 是带索引的双向块状链表
 * stack queue 对Deque的封装
 */
#include <deque>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    deque<int> dq1;
    deque<int> dq2(1);

    dq1.push_back(1);
    dq1.push_front(2); // 与vector的区别

    for (auto iter = dq1.begin(); iter != dq1.end(); iter++) {
        cout << *iter << " ";
    }
    cout << endl;

    return 0;
}
