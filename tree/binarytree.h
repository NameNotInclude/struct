#ifndef BINARYTREE_H
#define BINARYTREE_H

/*
 * 二叉搜索树（BST），支持构建、遍历（递归/非递归/层序）、
 * 查找、插入、删除，以及中序线索化与线索遍历。
 */
typedef struct BTNode {
    int data;
    struct BTNode *left;
    struct BTNode *right;
    int lflag; /* 1 表示 left 指向前驱线索 */
    int rflag; /* 1 表示 right 指向后继线索 */
} BTNode;

/* 由数组 a[0..n-1] 构建平衡 BST（会就地排序 a） */
BTNode* bt_build(int *a, int n);

BTNode* bt_insert(BTNode *t, int val);
BTNode* bt_delete(BTNode *t, int val);
BTNode* bt_find(BTNode *t, int val);
BTNode* bt_find_min(BTNode *t);
BTNode* bt_find_max(BTNode *t);
int     bt_depth(BTNode *t);

/* 递归遍历 */
void bt_preorder(BTNode *t);
void bt_inorder(BTNode *t);
void bt_postorder(BTNode *t);

/* 非递归遍历 */
void bt_preorder_iter(BTNode *t);
void bt_inorder_iter(BTNode *t);
void bt_postorder_iter(BTNode *t);

/* 层序遍历 */
void bt_levelorder(BTNode *t);

/* 中序线索化（会修改树结构）；之后请用 bt_free_threaded 释放 */
void bt_inorder_thread(BTNode *t);
/* 中序线索遍历 */
void bt_inorder_thread_traverse(BTNode *t);

/* 释放普通二叉树 */
void bt_free(BTNode *t);
/* 释放线索化后的二叉树 */
void bt_free_threaded(BTNode *t);

#endif /* BINARYTREE_H */
