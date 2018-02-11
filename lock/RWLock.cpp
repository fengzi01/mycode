// 读写锁
#include "RWLock.h"

class RWLock {
    private:
        pthread_mutex_t mxt;
        pthread_cond_t cond;
        int rd_cnt;//等待读的数量
        int wr_cnt;//等待写的数量

    public:
        RWLock() :rd_cnt(0), wr_cnt(0) {
            pthread_mutex_init(&mxt,NULL);
            pthread_cond_init(&cond,NULL);
        }

        void readLock() {
            pthread_mutex_lock(&mxt);

            ++rd_cnt;
            while(wr_cnt > 0)
                pthread_mutex_wait(&cond, &mxt);

            pthread_mutex_unlock(&mxt);
        }

        void readUnlock() {
            pthread_mutex_lock(&mxt);

            --rd_cnt;
            if (rd_cnt == 0 )
                pthread_cond_signal(&cond);

            pthread_mutex_unlock(&mxt);
        }

        void writeLock() {
            pthread_mutex_lock(&mxt);

            ++wr_cnt;
            while (wr_cnt + rd_cnt >=2)
                pthread_cond_wait(&cond, &mxt);

            pthread_mutex_unlock(&mxt);
        }

        void writerUnlock() {
            pthread_mutex_lock(&mxt);

            --wr_cnt;
            if(wr_cnt==0)
                pthread_cond_signal(&cond);

            pthread_mutex_unlock(&mxt);
        }
};
