// 一个基于c的队列实现
#ifndef CQUEUE_H_INCLUDED
#define CQUEUE_H_INCLUDED

typedef struct cnode* cnode_t;

// 节点结构
struct cnode {
    void *data; // 数据
    cnode_t next; // 下个节点
};

// 队列结构
typedef struct cqueue {
    struct cnode head;
    cnode_t tail;
} *cqueue_t;

// 队列标准函数
cqueue_t cq_create();
void* cq_enqueue(cqueue_t q,unsigned int bytes);
void* cq_dequeue(cqueue_t q);
void cq_destroy(cqueue_t q);
int cq_empty(cqueue_t q);

#endif // CQUEUE_H_INCLUDED


