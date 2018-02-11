#include <vector>

using std::vector;

int main(int argc, char *argv[])
{
    vector<int> v1; 
    v1.push_back(1);
    vector<int>::allocator_type type;

    v1.cbegin();
    v1.cend();
    v1.begin();
    v1.end();

    return 0;
}
