#include <iostream>
#include <string>
using namespace std;

class Person;
class Friend {
private:
    string name = "friend class";
public:
  // 直接能访问my类的所有成员变量。
  void SayMy(Person &p);
};

class Person {
private:
  int id;
  int weight;
public:
  Person():id(0),weight(0) {}
  Person(int id,int w):id(id),weight(w) {}
  friend void Friend::SayMy(Person &p);
};

void Friend::SayMy(Person &p) {
    cout << "my [id:" << p.id << "weight:" << p.weight << "]" << endl;
    cout << this->name << endl;
}

int main() {
    Person p(1,100);
    Friend f;
    f.SayMy(p);
}
