#include "Thread.h"

namespace std1 {
/* 使得C++编译器不添加 xxstd1xxnative_thread_routine修饰符 */
/* 所以这里不加也没有问题 */
extern "C" {
    static void* native_thread_routine(void* p) {
        Thread::StatePtr ptr{static_cast<Thread::State*>(p)};
        ptr->run();
        return nullptr;
    }
}

Thread::Thread(ThreadFunc &&callable):_joined(false) {
    // 启动线程
    startThread(StatePtr(new State(std::forward<ThreadFunc>(callable))));
}

void Thread::startThread(StatePtr state) {
    const int err = pthread_create(&_ptid,NULL,&native_thread_routine,state.get());
    if (err) {
        perror("pthread_create call fail!");
    }
    // FIXME why?
    state.release();
}

void Thread::join() {
    if (!_joined) {
        pthread_join(_ptid,NULL);
        _joined = true;
    }
}
void Thread::detach() {
    if (_joined) {
        fprintf(stderr,"detach already joined thread!");
        return;
    }
    pthread_detach(_ptid);
}
} // std1
