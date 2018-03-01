#ifndef _THREAD_H
#define _THREAD_H 
#include <functional>
#include <pthread.h>
#include <unistd.h>  // pid_t
#include <memory>
#include <utility> // std::forword

namespace std1 {

/**
 * pthread简单封装
 * 参照 c++11 std::thread
 */
class Thread {
    public:
       typedef std::function<void ()> ThreadFunc;
       explicit Thread(ThreadFunc &&callable):_joined(false) {
           // 启动线程
           startThread(StatePtr(new State(std::forward<ThreadFunc>(callable))));
       }

       ~Thread() {
           if (joinable()) {
               detach();
           }
       }

       Thread(Thread &) = delete;
       Thread(const Thread &) = delete;
       Thread& operator=(const Thread&) = delete;

       void join();
       void detach();

       bool joinable() const noexcept {
           return !_joined;
       }

       const pthread_t &getptid() const noexcept {
           return _ptid;
       }

       /* thread data 封装 */
       struct State {
           ThreadFunc _callable;
           State(ThreadFunc&& callable)
               :_callable(std::forward<ThreadFunc>(callable)) {}
           void run() {
               _callable();
           }
       };
       typedef std::unique_ptr<State> StatePtr;

    private:
       void startThread(StatePtr state);
       pthread_t _ptid;
       bool _joined;
       pid_t _pid;
};
} // std1
#endif /* ifndef _THREAD_H */