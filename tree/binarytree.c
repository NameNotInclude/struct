#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "binarytree.h"

#define BT_MIN(a, b) ((a) < (b) ? (a) : (b))

static void quick_sort(int arr[], int start, int end);
static int pivot(int arr[], int start, int end);
static BTNode* bt_build_inner(int *a, int start, int end);
static int position(int *a, int start, int end);
static void bt_thread_inner(BTNode *t, BTNode **pre);

/* ---------------- 构建 ---------------- */

BTNode* bt_build(int *a, int n)
{
    if (n <= 0)
        return NULL;
    quick_sort(a, 0, n - 1);
    return bt_build_inner(a, 0, n - 1);
}

static BTNode* bt_build_inner(int *a, int start, int end)
{
    if (start > end)
        return NULL;
    int p = position(a, start, end);
    BTNode *node = (BTNode*)malloc(sizeof(BTNode));
    node->data = a[p];
    node->lflag = 0;
    node->rflag = 0;
    node->left = bt_build_inner(a, start, p - 1);
    node->right = bt_build_inner(a, p + 1, end);
    return node;
}

/* 求完全二叉树根节点在有序数组中的位置 */
static int position(int *a, int start, int end)
{
    (void)a;
    int n = end - start + 1;
    int depth = (int)log2(n + 1);
    int last = n - (int)pow(2, depth) + 1;
    int left_n = (int)pow(2, depth - 1) - 1 + BT_MIN(last, (int)pow(2, depth - 1));
    return start + left_n;
}

/* ---------------- 查找 ---------------- */

int bt_depth(BTNode *t)
{
    if (t == NULL)
        return 0;
    int ld = (t->lflag == 0) ? bt_depth(t->left) : 0;
    int rd = (t->rflag == 0) ? bt_depth(t->right) : 0;
    return (ld > rd ? ld : rd) + 1;
}

BTNode* bt_find(BTNode *t, int val)
{
    while (t)
    {
        if (val == t->data)
            return t;
        if (val < t->data)
            t = (t->lflag == 0) ? t->left : NULL;
        else
            t = (t->rflag == 0) ? t->right : NULL;
    }
    return NULL;
}

BTNode* bt_find_min(BTNode *t)
{
    if (t == NULL)
        return NULL;
    while (t->lflag == 0 && t->left != NULL)
        t = t->left;
    return t;
}

BTNode* bt_find_max(BTNode *t)
{
    if (t == NULL)
        return NULL;
    while (t->rflag == 0 && t->right != NULL)
        t = t->right;
    return t;
}

/* ---------------- 递归遍历 ---------------- */

void bt_preorder(BTNode *t)
{
    if (t == NULL)
        return;
    printf("%d ", t->data);
    if (t->lflag == 0)
        bt_preorder(t->left);
    if (t->rflag == 0)
        bt_preorder(t->right);
}

void bt_inorder(BTNode *t)
{
    if (t == NULL)
        return;
    if (t->lflag == 0)
        bt_inorder(t->left);
    printf("%d ", t->data);
    if (t->rflag == 0)
        bt_inorder(t->right);
}

void bt_postorder(BTNode *t)
{
    if (t == NULL)
        return;
    if (t->lflag == 0)
        bt_postorder(t->left);
    if (t->rflag == 0)
        bt_postorder(t->right);
    printf("%d ", t->data);
}

/* ---------------- 非递归遍历 ---------------- */

void bt_preorder_iter(BTNode *t)
{
    if (t == NULL)
        return;
    int d = bt_depth(t);
    BTNode **stack = (BTNode**)malloc(sizeof(BTNode*) * d);
    int top = 0;
    stack[top++] = t;

    while (top > 0)
    {
        BTNode *cur = stack[--top];
        printf("%d ", cur->data);
        if (cur->rflag == 0 && cur->right != NULL)
            stack[top++] = cur->right;
        if (cur->lflag == 0 && cur->left != NULL)
            stack[top++] = cur->left;
    }
    free(stack);
}

void bt_inorder_iter(BTNode *t)
{
    if (t == NULL)
        return;
    int d = bt_depth(t);
    BTNode **stack = (BTNode**)malloc(sizeof(BTNode*) * d);
    int top = 0;
    BTNode *cur = t;

    while (cur != NULL || top > 0)
    {
        while (cur != NULL && cur->lflag == 0)
        {
            stack[top++] = cur;
            cur = cur->left;
        }
        if (cur != NULL)
            stack[top++] = cur;

        cur = stack[--top];
        printf("%d ", cur->data);
        cur = (cur->rflag == 0) ? cur->right : NULL;
    }
    free(stack);
}

void bt_postorder_iter(BTNode *t)
{
    if (t == NULL)
        return;
    int d = bt_depth(t);
    BTNode **stack = (BTNode**)malloc(sizeof(BTNode*) * d);
    int top = 0;
    BTNode *last = NULL;
    BTNode *cur = t;

    while (cur != NULL || top > 0)
    {
        while (cur != NULL && cur->lflag == 0)
        {
            stack[top++] = cur;
            cur = cur->left;
        }
        if (cur != NULL)
            stack[top++] = cur;

        cur = stack[top - 1];
        if (cur->rflag == 0 && cur->right != NULL && cur->right != last)
            cur = cur->right;
        else
        {
            top--;
            printf("%d ", cur->data);
            last = cur;
            cur = NULL;
        }
    }
    free(stack);
}

void bt_levelorder(BTNode *t)
{
    if (t == NULL)
        return;
    BTNode *queue[1001] = {0};
    int front = 0, rear = 0;
    queue[rear] = t;
    rear = (rear + 1) % 1001;

    while (front != rear)
    {
        BTNode *cur = queue[front];
        front = (front + 1) % 1001;
        printf("%d ", cur->data);
        if (cur->lflag == 0 && cur->left != NULL)
        {
            queue[rear] = cur->left;
            rear = (rear + 1) % 1001;
        }
        if (cur->rflag == 0 && cur->right != NULL)
        {
            queue[rear] = cur->right;
            rear = (rear + 1) % 1001;
        }
    }
}

/* ---------------- 线索化 ---------------- */

void bt_inorder_thread(BTNode *t)
{
    BTNode *pre = NULL;
    bt_thread_inner(t, &pre);
    if (pre != NULL)
    {
        pre->right = NULL;
        pre->rflag = 1;
    }
}

static void bt_thread_inner(BTNode *t, BTNode **pre)
{
    if (t == NULL)
        return;

    if (t->lflag == 0)
        bt_thread_inner(t->left, pre);

    if (t->left == NULL)
    {
        t->left = *pre;
        t->lflag = 1;
    }
    if (*pre != NULL && (*pre)->right == NULL)
    {
        (*pre)->right = t;
        (*pre)->rflag = 1;
    }
    *pre = t;

    if (t->rflag == 0)
        bt_thread_inner(t->right, pre);
}

void bt_inorder_thread_traverse(BTNode *t)
{
    BTNode *cur = t;
    if (cur == NULL)
        return;

    while (cur->lflag == 0)
        cur = cur->left;

    while (cur != NULL)
    {
        printf("%d ", cur->data);
        if (cur->rflag == 1)
            cur = cur->right;
        else
        {
            cur = cur->right;
            while (cur != NULL && cur->lflag == 0)
                cur = cur->left;
        }
    }
}

/* ---------------- 插入 / 删除 ---------------- */

BTNode* bt_insert(BTNode *t, int val)
{
    if (t == NULL)
    {
        t = (BTNode*)malloc(sizeof(BTNode));
        t->data = val;
        t->left = NULL;
        t->right = NULL;
        t->lflag = 0;
        t->rflag = 0;
    }
    else if (val < t->data)
    {
        t->left = bt_insert(t->left, val);
    }
    else if (val > t->data)
    {
        t->right = bt_insert(t->right, val);
    }
    return t;
}

BTNode* bt_delete(BTNode *t, int val)
{
    if (t == NULL)
        return NULL;

    if (val < t->data)
        t->left = bt_delete(t->left, val);
    else if (val > t->data)
        t->right = bt_delete(t->right, val);
    else
    {
        if (t->left != NULL && t->lflag == 0 && t->right != NULL && t->rflag == 0)
        {
            BTNode *tmp = bt_find_min(t->right);
            t->data = tmp->data;
            t->right = bt_delete(t->right, tmp->data);
        }
        else
        {
            BTNode *tmp = t;
            if (t->left == NULL && t->lflag == 0)
                t = t->right;
            else if (t->right == NULL && t->rflag == 0)
                t = t->left;
            else
                t = NULL;
            free(tmp);
        }
    }
    return t;
}

/* ---------------- 释放 ---------------- */

void bt_free(BTNode *t)
{
    if (t == NULL)
        return;
    if (t->lflag == 0)
        bt_free(t->left);
    if (t->rflag == 0)
        bt_free(t->right);
    free(t);
}

void bt_free_threaded(BTNode *t)
{
    if (t == NULL)
        return;
    if (t->lflag == 0)
        bt_free_threaded(t->left);
    if (t->rflag == 0)
        bt_free_threaded(t->right);
    free(t);
}

/* ---------------- 快速排序（构建用） ---------------- */

static void quick_sort(int arr[], int start, int end)
{
    if (start < end)
    {
        int p = pivot(arr, start, end);
        quick_sort(arr, start, p - 1);
        quick_sort(arr, p + 1, end);
    }
}

static int pivot(int arr[], int start, int end)
{
    int mid = (start + end) / 2;
    int tmp;

    if (arr[end] < arr[start])
    {
        tmp = arr[end]; arr[end] = arr[start]; arr[start] = tmp;
    }
    if (arr[end] < arr[mid])
    {
        tmp = arr[end]; arr[end] = arr[mid]; arr[mid] = tmp;
    }
    if (arr[mid] > arr[start])
    {
        tmp = arr[mid]; arr[mid] = arr[start]; arr[start] = tmp;
    }

    int p = arr[start];
    int low = start;
    int high = end;

    while (low < high)
    {
        while (low < high && arr[high] >= p) high--;
        while (low < high && arr[low] <= p) low++;
        tmp = arr[low];
        arr[low] = arr[high];
        arr[high] = tmp;
    }

    arr[start] = arr[low];
    arr[low] = p;
    return low;
}
