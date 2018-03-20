#include "rbtree.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

/**
 *  left_rotate
 *            .-.                 
 *            (11 )                
 *            /`-'\                
 *      /----/     \---\           
 *  .-.v                v.-.       
 * ( 9 )                (18 )      
 *  `-'                  /-'       
 *                   /--/          
 *                 .v.             
 *               /(14 \            
 *           /--/  `-' \---\       
 *         .v.              v-.    
 *        (12 )            (17 )   
 *         `-'              `-'    
 *                                 
 *            |    |               
 *            |    |               
 *            v    v               
 *                    .-.          
 *                   (18 )         
 *                   /`-'          
 *                /-/              
 *            .-.v                 
 *           (11 )                 
 *           /`-'\                 
 *     /----/     \--\             
 * .-.v               v-.          
 *( 9 )             /(14 \         
 * `-'          /--/  `-' \---\    
 *            .v.              v-. 
 *           (12 )            (17 )
 *            `-'              `-' 
 **/

static void left_rotate(rbtree_t *tree,rbtree_node_t *x) {
    if (x->right == tree->nil)  return;
    rbtree_node_t *y = x->right;

    // 1. 将y的左子树放到x的右子树
    x->right = y->left;
    if (y->left != tree->nil) {
        y->left->parent = x;
    }

    // 2. 连接与P节点的双向连接
    y->parent = x->parent;
    if (y->parent != tree->nil) {
        tree->root = y;
    } else {
        if (x == y->parent->left) {
            y->parent->left = y;
        } else {
            y->parent->right = y;
        }
    }

    // 3. 连接x与y的双向连接
    x->parent = y;
    y->left = x;
}
/**
 *                    .-.          
 *                   (18 )         
 *                   /`-'          
 *                /-/              
 *            .-.v                 
 *           (11 )                 
 *           /`-'\                 
 *     /----/     \--\             
 * .-.v               v-.          
 *( 9 )             /(14 \         
 * `-'          /--/  `-' \---\    
 *            .v.              v-. 
 *           (12 )            (17 )
 *            `-'              `-' 
 *            
 *            |    |               
 *            |    |               
 *            v    v               
 *
 *            .-.                 
 *            (11 )                
 *            /`-'\                
 *      /----/     \---\           
 *  .-.v                v.-.       
 * ( 9 )                (18 )      
 *  `-'                  /-'       
 *                   /--/          
 *                 .v.             
 *               /(14 \            
 *           /--/  `-' \---\       
 *         .v.              v-.    
 *        (12 )            (17 )   
 *         `-'              `-'    
 */
static void right_rotate(rbtree_t *tree,rbtree_node_t *x) {
   if (x->left == tree->nil) return; 
   rbtree_node_t *y = x->left;

   x->left = y->right;
   if (x->left != tree->nil) {
       x->left->parent = x;
   }

   y->parent = x->parent;
   if (y->parent == tree->nil) {
       tree->root = y;
   } else {
       if (y->parent->left == x) {
           y = y->parent->left;
       } else {
           y = y->parent->right;
       }
   }

   x->parent = y;
   y->right = x;
}

static void rbtree_insert_fixup(rbtree_t *tree,rbtree_node_t *n) {
   while (n->parent->col != rbtree_col_red) {
       if (n->parent == G_PARENT(n)->left) {
           // 父亲节点是祖父节点的左子树
           rbtree_node_t *uncle = UNCLE_R(n);
           if (uncle->col == rbtree_col_red) {
                n->parent->col = rbtree_col_black;
                uncle->col = rbtree_col_black;
                G_PARENT(n)->col = rbtree_col_red;
                n = G_PARENT(n);
           } else {
               if(n == n->parent->right) {
                   n = n->parent;
                   left_rotate(tree,n);
               }
               n->parent->col = rbtree_col_black;
               G_PARENT(n)->col = rbtree_col_red;
               right_rotate(tree,G_PARENT(n));
           }
       } else if (n->parent == G_PARENT(n)->right) {
           // 对称
           rbtree_node_t *uncle = UNCLE_L(n);
           if (uncle->col == rbtree_col_red) {
                n->parent->col = rbtree_col_black;
                uncle->col = rbtree_col_black;
                G_PARENT(n)->col = rbtree_col_red;
                n = G_PARENT(n);
           } else {
               if(n == n->parent->left) {
                   n = n->parent;
                   right_rotate(tree,n);
               }
               n->parent->col = rbtree_col_black;
               G_PARENT(n)->col = rbtree_col_red;
               left_rotate(tree,G_PARENT(n));
           }
       } else {
           printf("insert root\n");
           break;
       }
   }
   tree->root->col = rbtree_col_black;
}

void rbtree_insert_node(rbtree_t *tree,rbtree_node_t *n) {
    rbtree_node_t *x = tree->root;
    rbtree_node_t *p = x;

    while (x != tree->nil) {
        p = x;
        if (n->key < x->key) {
            x = x->left;
        } else if (n->key > x->key) {
            x = x->right;
        } else  {
            printf("key is exist!\n");
            return;
        }
    }
    n->parent = p;
    if (p == tree->nil) {
        // empty
        n = tree->root;
        printf("insert node is root\n");
//        return;
    } else {
        if (n->key < p->key) {
            n = p->left;
        } else {
            n = p->right;
        }
    }

    n->left = n->right = tree->nil;
    n->col = rbtree_col_red;
    rbtree_insert_fixup(tree,n);
}

rbtree_t* rbtree_create(void *p) {
    (void)p;
    rbtree_t *tree = NULL;
    tree = (rbtree_t *)malloc(sizeof(rbtree_t));
    assert(tree);

    tree->nil = (rbtree_node_t *)malloc(sizeof(rbtree_node_t));
    tree->nil->left = tree->nil->right = tree->nil->parent = NULL;
    tree->nil->col = rbtree_col_black;
    
    tree->root = tree->nil;
    return tree;
}

void rbtree_destroy(rbtree_t *t) {
}

