#include <stdlib.h>
#include "hash.h"

/* 开放定址法：线性探测 / 平方探测 / 双散列 */

typedef struct {
    int value;
    short state; /* 0 空, 1 占用, -1 已删除 */
} OASlot;

struct OAHash {
    OASlot *slots;
    int capacity;
    int mode; /* 0 线性, 1 平方, 2 双散列 */
};

static int probe(const OAHash *h, int key, int i)
{
    int pos = hash_default(key, h->capacity);
    switch (h->mode)
    {
        case 0:
            return (pos + i) % h->capacity;
        case 1:
            return (pos + i * i) % h->capacity;
        case 2:
        {
            int d = (h->capacity > 1) ? 1 + (key % (h->capacity - 1)) : 1;
            return (pos + i * d) % h->capacity;
        }
        default:
            return -1;
    }
}

OAHash* oa_create(int capacity, int mode)
{
    if (capacity <= 0)
        return NULL;
    if (mode < 0 || mode > 2)
        mode = 0;
    OAHash *h = (OAHash*)malloc(sizeof(OAHash));
    h->slots = (OASlot*)malloc(sizeof(OASlot) * capacity);
    for (int i = 0; i < capacity; i++)
        h->slots[i].state = 0;
    h->capacity = capacity;
    h->mode = mode;
    return h;
}

void oa_insert(OAHash *h, int value)
{
    if (!h || !h->slots)
        return;

    int first_deleted = -1;
    for (int i = 0; i < h->capacity; i++)
    {
        int idx = probe(h, value, i);
        if (idx < 0)
            return;

        if (h->slots[idx].state == 0)
        {
            int target = (first_deleted >= 0) ? first_deleted : idx;
            h->slots[target].value = value;
            h->slots[target].state = 1;
            return;
        }

        if (h->slots[idx].state == -1 && first_deleted < 0)
            first_deleted = idx;

        if (h->slots[idx].state == 1 && h->slots[idx].value == value)
            return;
    }

    if (first_deleted >= 0)
    {
        h->slots[first_deleted].value = value;
        h->slots[first_deleted].state = 1;
    }
}

int oa_search(OAHash *h, int value)
{
    if (!h || !h->slots)
        return -1;

    for (int i = 0; i < h->capacity; i++)
    {
        int idx = probe(h, value, i);
        if (idx < 0)
            return -1;

        if (h->slots[idx].state == 0)
            return -1;

        if (h->slots[idx].state == 1 && h->slots[idx].value == value)
            return idx;
    }
    return -1;
}

void oa_delete(OAHash *h, int value)
{
    if (!h || !h->slots)
        return;
    int idx = oa_search(h, value);
    if (idx >= 0)
        h->slots[idx].state = -1;
}

void oa_free(OAHash *h)
{
    if (!h)
        return;
    free(h->slots);
    free(h);
}
