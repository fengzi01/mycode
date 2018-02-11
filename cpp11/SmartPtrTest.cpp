// 智能指针
#include <memory>
using std::shared_ptr;

int main(int argc, char *argv[])
{
    shared_ptr<int> p1(new int(10));
    return 0;
}
