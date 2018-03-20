#include "rbtree.h"
#include "stdio.h"
#include "stdlib.h"

void rbtree_travel(rbtree_node_t *n,rbtree_node_t *nil) {
    if (nil == n) {
        return;
    }
    rbtree_travel(n->left,nil);
    printf("%d ",n->key);
    rbtree_travel(n->right,nil);
}

void TEST_rbtree_travel(rbtree_t *tree) {
    rbtree_node_t *root = tree->root;
    rbtree_travel(root->left,tree->nil);
}

int main() {
    rbtree_t *t = rbtree_create(NULL);
    int i = 0;
    for (i=0;i<1;++i) {
        rbtree_node_t *n = (rbtree_node_t *)malloc(sizeof(rbtree_node_t));
        INIT_RBTREE_NODE(n,t->nil,i,NULL);
        rbtree_insert_node(t,n);
    }

    TEST_rbtree_travel(t);

    rbtree_destroy(t);
}
