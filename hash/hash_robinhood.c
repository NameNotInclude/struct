#include <stdlib.h>
#include "hash.h"

/* Robin Hood 哈希：记录探测距离 dist，插入时“劫富济贫” */

typedef struct {
    int value;
    int dist;
    short state; /* 0 空, 1 占用 */
} RHSlot;

struct RHHash {
    RHSlot *slots;
    int capacity;
};

RHHash* rh_create(int capacity)
{
    if (capacity <= 0)
        return NULL;
    RHHash *h = (RHHash*)malloc(sizeof(RHHash));
    h->slots = (RHSlot*)malloc(sizeof(RHSlot) * capacity);
    for (int i = 0; i < capacity; i++)
    {
        h->slots[i].state = 0;
        h->slots[i].dist = 0;
    }
    h->capacity = capacity;
    return h;
}

void rh_insert(RHHash *h, int value)
{
    if (!h || !h->slots)
        return;

    int pos = hash_default(value, h->capacity);
    RHSlot cur;
    cur.value = value;
    cur.dist = 0;
    cur.state = 1;

    for (int i = 0; i < h->capacity; i++)
    {
        int idx = (pos + cur.dist) % h->capacity;

        if (h->slots[idx].state == 0)
        {
            h->slots[idx] = cur;
            return;
        }

        if (h->slots[idx].value == value)
            return;

        if (h->slots[idx].dist < cur.dist)
        {
            RHSlot tmp = h->slots[idx];
            h->slots[idx] = cur;
            cur = tmp;
        }

        cur.dist++;
    }
}

int rh_search(RHHash *h, int value)
{
    if (!h || !h->slots)
        return -1;

    int pos = hash_default(value, h->capacity);
    for (int dist = 0; dist < h->capacity; dist++)
    {
        int idx = (pos + dist) % h->capacity;

        if (h->slots[idx].state == 0)
            return -1;

        if (h->slots[idx].dist < dist)
            return -1;

        if (h->slots[idx].value == value)
            return idx;
    }
    return -1;
}

void rh_delete(RHHash *h, int value)
{
    if (!h || !h->slots)
        return;

    int idx = rh_search(h, value);
    if (idx < 0)
        return;

    int next = (idx + 1) % h->capacity;
    while (h->slots[next].state == 1 && h->slots[next].dist > 0)
    {
        h->slots[idx] = h->slots[next];
        h->slots[idx].dist--;
        idx = next;
        next = (next + 1) % h->capacity;
    }

    h->slots[idx].state = 0;
    h->slots[idx].dist = 0;
}

void rh_free(RHHash *h)
{
    if (!h)
        return;
    free(h->slots);
    free(h);
}
