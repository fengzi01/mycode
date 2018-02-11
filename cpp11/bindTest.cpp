#include <functional>
#include <string>
#include <map>

using namespace std;

bool checkSize(const string &s,string::size_type sz) {
    return s.size() >= sz;
}

int main(int argc, char *argv[])
{
    map<int,int> m1;
    auto check6 = bind(checkSize,placeholders::_1,6);
    string s("hello");
    bool b1 = check6(s);
    return 0;
}
