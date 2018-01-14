/**  
 * 多个工作线程通过队列获取主线程分配给它们的作业
 * apue P330
 * rwlock实现
 *
 * 图见P309
 *                                .---.                                               
 *   .---.                       /     \ 
 *  /     \                     ; main  :                                             
 * ;worker :                    :thread ;                                             
 * :thread1;^                    \     /                                              
 *  \     /  \                    \   /                                               
 *   \   /    \                    `-'                                                
 *    `-'      \                    /                                                 
 *              \          /-------/                                                  
 *               X--------v                                                           
 *  .---.        |        |                                                           
 * /     \       |        |       +-------+      +-------+     +-------+     +-------+
 *; woker :      |        |       |       |      |       |     |       |     |       |
 *:thread2;<-----+ worker |       | TID1  |      | TID1  |     | TID3  |     | TID2  |
 * \     /       | queue  |<------|  job  |<-----|  job  <-----|  job  |<----|  job  |
 *  \   /        |        |       |       |      |       |     |       |     |       |
 *   `-'         |        |       +-------+      +-------+     +-------+     +-------+
 *               |        |                                                           
 *               X--------+                                                           
 *   .---.      /                                                                     
 *  /     \   //                                                                      
 * ;worker : /                                                                        
 * :thread3;v                                                                         
 *  \     /                                                                           
 *   \   /                                                                            
 *    `-'                                                                             
 */

#include <pthread.h>
#include <assert.h>

/* job */
typedef struct _job_t {
    _job_t *next_;
    _job_t *prev_; // 双向链表
    pthread_t tid_; // 允许持有job的thread id

    void *data_; // 数据
    void (*ptr_)(void *data); // 调用函数
} Job;

class JobQueue {
    private:
        Job *head_;
        Job *tail_;
        pthread_rwlock_t rwlock_; // 读写锁
    public:
        JobQueue();
        ~JobQueue();

        void InsertJob(Job *job);
        void AppendJob(Job *job);
        void RemoveJob(Job *job);
        Job *FindJob(pthread_t tid);
};

JobQueue::JobQueue():head_(NULL),tail_(NULL) {
    pthread_rwlock_init(&rwlock_,NULL); // 初始化读写锁
}
JobQueue::~JobQueue() {
    pthread_rwlock_destroy(&rwlock_);
}

void JobQueue::InsertJob(Job *job) {
    assert(job != NULL);
    pthread_rwlock_wrlock(&rwlock_);

    job->next_ = head_;
    job->prev_ = NULL;

    if (head_ != NULL) {
        head_->prev_ = job;
    } else {
        tail_ = job; // empty
    }
    head_ = job;
    pthread_rwlock_unlock(&rwlock_);
}

void JobQueue::AppendJob(Job *job) {
    assert(job != NULL);
    pthread_rwlock_wrlock(&rwlock_);

    job->prev_ = tail_;
    if (tail_ != NULL) {
        tail_->next_ = job;
    } else {
        head_ = job;
    }

    tail_ = job;
    pthread_rwlock_unlock(&rwlock_);
}

void JobQueue::RemoveJob(Job *job) {
    assert(job != NULL);
    pthread_rwlock_wrlock(&rwlock_);
    // TODO
    pthread_rwlock_unlock(&rwlock_);
}

Job *JobQueue::FindJob(pthread_t tid) {
    assert(tid != 0);
    if (pthread_rwlock_rdlock(&rwlock_)!=0) {
        return NULL;
    }

    Job *j = head_;
    for (;j != NULL;j = j->next_) {
        if (pthread_equal(tid,j->tid_)) {
            break;
        }
    }

    pthread_rwlock_rdlock(&rwlock_);
    return j;
}

int main(int argc, char *argv[])
{
    // 严格只读的情况是不是就不用加锁了。。。 
    return 0;
}
