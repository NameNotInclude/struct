#include <stdlib.h>
#include "heap.h"

static void swap_int(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

static void percolate_down(int *data, int size, int i)
{
    int pos = i;
    int child;
    while (pos * 2 <= size)
    {
        child = pos * 2;
        if (child + 1 <= size && data[child + 1] < data[child])
            child++;
        if (data[pos] <= data[child])
            break;
        swap_int(&data[pos], &data[child]);
        pos = child;
    }
}

static void percolate_up(int *data, int i)
{
    int pos = i;
    while (pos > 1)
    {
        int parent = pos / 2;
        if (data[pos] >= data[parent])
            break;
        swap_int(&data[pos], &data[parent]);
        pos = parent;
    }
}

Heap* heap_create(int capacity)
{
    if (capacity < 0)
        capacity = 0;
    Heap *h = (Heap*)malloc(sizeof(Heap));
    h->data = (int*)malloc(sizeof(int) * (capacity + 1));
    h->size = 0;
    h->capacity = capacity;
    return h;
}

void heap_free(Heap *h)
{
    if (!h)
        return;
    free(h->data);
    free(h);
}

void heap_build(Heap *h, const int *values, int n)
{
    if (!h || n < 0)
        return;
    if (n > h->capacity)
    {
        int *nd = (int*)realloc(h->data, sizeof(int) * (n + 1));
        if (!nd)
            return;
        h->data = nd;
        h->capacity = n;
    }
    for (int i = 1; i <= n; i++)
        h->data[i] = values[i - 1];
    h->size = n;
    for (int i = n / 2; i >= 1; i--)
        percolate_down(h->data, h->size, i);
}

void heap_insert(Heap *h, int value)
{
    if (!h)
        return;
    if (h->size >= h->capacity)
    {
        int nc = h->capacity == 0 ? 4 : h->capacity * 2;
        int *nd = (int*)realloc(h->data, sizeof(int) * (nc + 1));
        if (!nd)
            return;
        h->data = nd;
        h->capacity = nc;
    }
    h->data[++h->size] = value;
    percolate_up(h->data, h->size);
}

int heap_delete_min(Heap *h)
{
    if (!h || h->size == 0)
        return -1;
    int min = h->data[1];
    h->data[1] = h->data[h->size--];
    percolate_down(h->data, h->size, 1);
    return min;
}

int heap_min(const Heap *h)
{
    return (!h || h->size == 0) ? -1 : h->data[1];
}

int heap_size(const Heap *h)
{
    return h ? h->size : 0;
}

int heap_empty(const Heap *h)
{
    return (!h || h->size == 0);
}

void heap_sort(int *array, int n)
{
    if (n <= 0)
        return;
    Heap *h = heap_create(n);
    heap_build(h, array, n);
    for (int i = 0; i < n; i++)
        array[i] = heap_delete_min(h);
    heap_free(h);
}
