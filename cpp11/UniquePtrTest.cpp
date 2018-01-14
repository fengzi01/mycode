#include <iostream>
#include <memory>
using namespace std;

class ClassA {
    private:
        int _data;
    public:
        ClassA(int data):_data(data) {
            cout << "ClassA Construct:" << _data << endl;
        }
        ~ClassA() {
            cout << "ClassA Destroy:" << _data << endl;
        }
};

int main(int argc, char *argv[])
{
    {
        unique_ptr<ClassA> p(new ClassA(1));
    }
    return 0;
}
