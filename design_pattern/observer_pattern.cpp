/**
 * 观察者模式(observer pattern)
 * 又叫发布-订阅模式(publish-subscribe)
 */
#include <list>
#include <string>
#include <iostream>
#include <vector>

using std::list;
using std::string;
using std::vector;

/**
 * 消息体
 */
typedef string Message;

/**
 * 观察者
 */
class Observer {
    public:
       virtual void update(Message message) = 0;
};

/**
 * 对象/被观察者
 * observable
 */
class Object {
    public:
        /* 注册observer */
        virtual void attach(Observer *ob) = 0;
        /* 删除observer */
        virtual bool detach(Observer *ob) = 0;
        /* 通知 */
        virtual void notify(Message message) = 0;
};

//
// 具体场景下
//

/**
 * 微信公众号
 * 供用户订阅
 */
class WeixinMP : public Object {
    private:
        list<Observer*> observers;
        vector<int> ss;
    public:
        WeixinMP() {
            observers.clear();
        }
        virtual void attach(Observer *ob) {
            observers.push_back(ob);
        }

        virtual bool detach(Observer *ob) {
            list<Observer*>::iterator iter;
            bool succ = false;
            for (iter=observers.begin();iter!=observers.end();++iter) {
                if (*iter == ob) {
                    observers.erase(iter);
                    succ = true;
                }
            }

            return succ;
        }

        virtual void notify(Message message) {
            for (list<Observer*>::iterator iter = observers.begin();iter != observers.end(); ++iter) {
                (*iter)->update(message);
            }
        }
};

class WeixinUser : public Observer {
    private:
        string name_;
    public:
        WeixinUser(string name):name_(name) {}
       virtual void update(Message message) {
           std::cout << "[" << name_  << "] 收到更新消息: [" << message << "]" << std::endl;
       }
};

int main(int argc, char *argv[])
{
    WeixinMP mp; 
    WeixinUser user1("张三");
    WeixinUser user2("李四");
    WeixinUser user3("王五");

    mp.attach(&user1);
    mp.attach(&user2);
    mp.attach(&user3);

    mp.notify("公众号更新了");

    mp.detach(&user2);

    mp.notify("公众号又又又更新了");
    return 0;
}



