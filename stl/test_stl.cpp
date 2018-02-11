#include<stack>
#include<iostream>
#include<vector>
#include<list>

int main(int argc, char *argv[])
{
    std::string s;
    std::cout << "end" << std::endl;
    std::cout << "hello world\n";
    std::vector<int> arr(10);
    arr.push_back(1);
    arr.push_back(2);
    arr.push_back(3);
    for (int i:arr) {
        std::cout << i << std::endl;
    }
    auto i = arr;
    std::cout << &i << std::endl;
    return 0;
}

