#include <stdlib.h>
#include "hash.h"

/* 分离链接法：每个桶是一条单链表 */

typedef struct CHNode {
    int value;
    struct CHNode *next;
} CHNode;

struct CHHash {
    CHNode **buckets;
    int capacity;
};

CHHash* ch_create(int capacity)
{
    if (capacity <= 0)
        return NULL;
    CHHash *h = (CHHash*)malloc(sizeof(CHHash));
    h->buckets = (CHNode**)calloc(capacity, sizeof(CHNode*));
    h->capacity = capacity;
    return h;
}

void ch_insert(CHHash *h, int value)
{
    if (!h || !h->buckets)
        return;
    if (ch_search(h, value))
        return; /* 不重复插入 */
    int pos = hash_default(value, h->capacity);
    CHNode *node = (CHNode*)malloc(sizeof(CHNode));
    node->value = value;
    node->next = h->buckets[pos];
    h->buckets[pos] = node;
}

int ch_search(CHHash *h, int value)
{
    if (!h || !h->buckets)
        return 0;
    int pos = hash_default(value, h->capacity);
    CHNode *cur = h->buckets[pos];
    while (cur)
    {
        if (cur->value == value)
            return 1;
        cur = cur->next;
    }
    return 0;
}

void ch_delete(CHHash *h, int value)
{
    if (!h || !h->buckets)
        return;
    int pos = hash_default(value, h->capacity);
    CHNode *prev = NULL;
    CHNode *cur = h->buckets[pos];
    while (cur)
    {
        if (cur->value == value)
        {
            if (prev)
                prev->next = cur->next;
            else
                h->buckets[pos] = cur->next;
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void ch_free(CHHash *h)
{
    if (!h)
        return;
    for (int i = 0; i < h->capacity; i++)
    {
        CHNode *cur = h->buckets[i];
        while (cur)
        {
            CHNode *tmp = cur->next;
            free(cur);
            cur = tmp;
        }
    }
    free(h->buckets);
    free(h);
}
