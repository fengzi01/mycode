#include <map>
#include <utility>
using std::map;

template<class T,class V>
class Map {
    public:
        typedef T key_type;
        typedef V value_type;
};

int main(int argc, char *argv[])
{
    Map<int,int>::key_type tt;
    map<int,int> map1;
    std::pair<int,bool> ret = map1.insert(std::pair<int,int>(1,1));
    return 0;
}


