#ifndef ZIGZAGGINGTREE_H
#define ZIGZAGGINGTREE_H

/*
 * 由中序 + 后序序列重建二叉树，并进行之字形（Zigzag）层序遍历。
 */
typedef struct ZNode {
    int data;
    struct ZNode *left;
    struct ZNode *right;
} ZNode;

/* 用 inorder[0..n-1] 和 postorder[0..n-1] 重建二叉树 */
ZNode* zigzag_build(const int *inorder, const int *postorder, int n);

/* 之字形层序遍历打印（第 0 层从左到右，第 1 层从右到左，依此类推） */
void zigzag_print(const ZNode *root);

void zigzag_free(ZNode *t);

#endif /* ZIGZAGGINGTREE_H */
