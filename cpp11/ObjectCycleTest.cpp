#include <iostream>
#include <memory>
using namespace std;

class TestA {
    private:
        char _data;
    public:
        TestA(char i) {
            _data = i;
            cout << "construct TestA:" << _data << endl;
        }
        ~TestA() {
            cout << "destroy TestA:" << _data << endl;
        }
};

int main() {
    TestA a('b');
    TestA *b = new TestA('c');
    delete b;
    cout << "main start" << endl;
    {
        TestA a('a');
    }
    cout << "main ret" << endl;
}
