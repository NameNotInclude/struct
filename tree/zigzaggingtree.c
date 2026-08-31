#include <stdio.h>
#include <stdlib.h>
#include "zigzaggingtree.h"

static int find_index(const int *arr, int n, int target)
{
    for (int i = 0; i < n; i++)
        if (arr[i] == target)
            return i;
    return -1;
}

static ZNode* zigzag_build_inner(const int *in, const int *post,
                                 int instart, int inend,
                                 int poststart, int postend)
{
    if (instart > inend)
        return NULL;

    ZNode *node = (ZNode*)malloc(sizeof(ZNode));
    node->data = post[postend];

    int pos = find_index(in + instart, inend - instart + 1, post[postend]);
    node->left = zigzag_build_inner(in, post,
                                    instart, instart + pos - 1,
                                    poststart, poststart + pos - 1);
    node->right = zigzag_build_inner(in, post,
                                     instart + pos + 1, inend,
                                     poststart + pos, postend - 1);
    return node;
}

ZNode* zigzag_build(const int *inorder, const int *postorder, int n)
{
    if (n <= 0)
        return NULL;
    return zigzag_build_inner(inorder, postorder, 0, n - 1, 0, n - 1);
}

static int count_nodes(const ZNode *t)
{
    if (t == NULL)
        return 0;
    return 1 + count_nodes(t->left) + count_nodes(t->right);
}

void zigzag_print(const ZNode *root)
{
    if (root == NULL)
        return;

    int n = count_nodes(root);
    ZNode **queue = (ZNode**)malloc(sizeof(ZNode*) * n);
    int *level = (int*)malloc(sizeof(int) * n);

    /* 每个结点最多 n 个，用二维数组按层收集值 */
    int **values = (int**)malloc(sizeof(int*) * n);
    int *cnt = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < n; i++)
        values[i] = (int*)malloc(sizeof(int) * n);

    int front = 0, rear = 0;
    queue[rear] = (ZNode*)root;
    level[rear] = 0;
    rear++;

    int maxlevel = 0;
    while (front < rear)
    {
        ZNode *cur = queue[front];
        int lv = level[front];
        front++;

        values[lv][cnt[lv]++] = cur->data;
        if (lv > maxlevel)
            maxlevel = lv;

        if (cur->left)
        {
            queue[rear] = cur->left;
            level[rear] = lv + 1;
            rear++;
        }
        if (cur->right)
        {
            queue[rear] = cur->right;
            level[rear] = lv + 1;
            rear++;
        }
    }

    /* 与原始程序一致：第 0 层从左到右，第 1 层从左到右，
     * 第 2 层从右到左，第 3 层从左到右……（偶数层反向） */
    for (int lv = 0; lv <= maxlevel; lv++)
    {
        if (lv > 0 && lv % 2 == 0)
        {
            for (int j = cnt[lv] - 1; j >= 0; j--)
                printf("%d ", values[lv][j]);
        }
        else
        {
            for (int j = 0; j < cnt[lv]; j++)
                printf("%d ", values[lv][j]);
        }
    }
    printf("\n");

    for (int i = 0; i < n; i++)
        free(values[i]);
    free(values);
    free(cnt);
    free(queue);
    free(level);
}

void zigzag_free(ZNode *t)
{
    if (t == NULL)
        return;
    zigzag_free(t->left);
    zigzag_free(t->right);
    free(t);
}
