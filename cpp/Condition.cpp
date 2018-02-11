#include <pthread.h>
#include <stddef.h>

class Condition {
    public: 
        Condition():_raw(PTHREAD_COND_INITIALIZER){}
        ~Condition() {
            pthread_cond_destroy(&_raw);
        }
        void wait(pthread_mutex_t &mutex) {
            pthread_mutex_lock(&mutex);
            while (_cond) {
                pthread_cond_wait(&_raw,&mutex);
            }
            pthread_mutex_unlock(&mutex);
        }

        void notify(pthread_mutex_t &mutex) {
            pthread_mutex_lock(&mutex);
            _cond = !_cond;
            pthread_cond_signal(&_raw);
            pthread_mutex_unlock(&mutex);
        }
        void broadcast() {
            //pthread_cond_broadcast(&_raw);
        }
        void wait_until(pthread_mutex_t mutex,size_t microseconds);
    private:
        pthread_cond_t _raw;
        bool _cond;
};
