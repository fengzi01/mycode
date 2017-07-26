#include "cqueue.h"
#include "stdlib.h"

cqueue_t cq_create() {
    cqueue_t q;
    // 创建队列
    q = (cqueue_t)malloc(sizeof(struct cqueue));
    q->head.data = NULL;
    q->head.next = NULL;
    q->tail = &q->head;

    return q;
}

void* cq_enqueue(cqueue_t q,unsigned int bytes) {
    cnode_t node = (cnode_t)malloc(sizeof(struct cnode));
    node->data = (void *)malloc(bytes);
    node->next = NULL;
    // 将节点加入到尾结点
    q->tail->next = node;
    q->tail = node;
    return node->data;
}

void* cq_dequeue(cqueue_t q) {
    if (q->tail == &q->head) {
        return NULL;
    }
    cnode_t tmp = q->head.next;
    void *data = tmp->data;

    // 指针指向下一个节点
    q->head.next = tmp->next;
    if (tmp->next == NULL) {
        q->tail = &q->head;
    }
    free(tmp);
    
    return data;
}

void cq_destroy(cqueue_t q) {
    cnode_t p = q->head.next;
    // 顺序释放节点和数据内存
    while (p != NULL) {
        cnode_t tmp = p;
        p = p->next;
        free(tmp->data);
        free(tmp);
    }

    free(q);
}

int cq_empty(cqueue_t q) {
    return q->tail == &q->head;
}
