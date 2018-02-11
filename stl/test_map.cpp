#include<map>
#include<set>
#include<string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    map<int,string> map1; 
    map<int,int> map2;

    map1[3] = "nijia"; 
    map1.insert(map<int,string>::value_type(1,"nijia master")); // 插入
    string data = map1[3];

    auto iter = map1.begin();
    int key = iter->first;
    string value = iter->second;

    cout << "key:" << key << " value:" << value << endl;

    map1.erase(map1.begin());
    map1.erase(1);

    map1.size();
    map1.empty();
    map1.clear();

    map1[1] = "hello";
    map1[2] = "world";

    for (auto iter = map1.begin(); iter != map1.end(); iter++) {
        cout << iter->first << ";" << iter->second << endl;
    }


    // set 
    cout << "=================================" << endl;

    set<int> set1;
    set1.insert(1);
    set1.insert(1);
    set1.insert(3);

    for (auto iter = set1.begin();iter != set1.end();iter++) {
        cout << *iter << endl;
    }

    cout << set1.size() << endl;

    return 0;
}
