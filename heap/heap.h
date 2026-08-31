#ifndef HEAP_H
#define HEAP_H

/*
 * 最小堆（二叉堆），内部使用下标从 1 开始的数组存储。
 */
typedef struct {
    int *data;     /* data[1..size] 为有效元素 */
    int size;
    int capacity;
} Heap;

/* 创建容量为 capacity 的堆（capacity 可自动扩容） */
Heap* heap_create(int capacity);

/* 释放堆 */
void heap_free(Heap *h);

/* 用 values[0..n-1] 建堆（会覆盖堆中原有内容） */
void heap_build(Heap *h, const int *values, int n);

/* 插入元素 */
void heap_insert(Heap *h, int value);

/* 删除并返回最小元素；堆为空时返回 -1 */
int heap_delete_min(Heap *h);

/* 查看最小元素（不删除）；堆为空时返回 -1 */
int heap_min(const Heap *h);

int heap_size(const Heap *h);
int heap_empty(const Heap *h);

/* 堆排序：将 array[0..n-1] 原地升序排序 */
void heap_sort(int *array, int n);

#endif /* HEAP_H */
