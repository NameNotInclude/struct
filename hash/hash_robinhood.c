#include <stdio.h>
#include <stdlib.h>

typedef int (*hashf)(int);

typedef struct
{
    int value;
    int dist;
    short state;
    //0:empty,1:normal
} hashnode;

typedef struct
{
    hashnode* list;
    int capacity;
    hashf func;
} hash;

int capa;

hash* creat(int c, hashf fun);
int default_hash_function(int key);
void insert(hash* H, int value);
void delete_node(hash* H, int value);
int search(hash* H, int value);
void freeH(hash* H);

int main()
{
    hash* h = creat(23, NULL);
    if (h == NULL)
        return 0;

    for (int i = 0; i < 7; i++)
    {
        int x;
        scanf("%d", &x);
        insert(h, x);
    }

    for (int i = 0; i < h->capacity; i++)
        printf("%4d ", i);
    printf("\n");

    for (int i = 0; i < h->capacity; i++)
    {
        if (h->list[i].state == 1)
            printf("%4d ", h->list[i].value);
        else
            printf("  -1 ");
    }
    printf("\n");

    freeH(h);
    return 0;
}

hash* creat(int c, hashf fun)
{
    if (c <= 0)
        return NULL;

    hashnode* h = (hashnode*)malloc(sizeof(hashnode) * c);
    if (h == NULL)
        return NULL;

    for (int i = 0; i < c; i++)
    {
        h[i].state = 0;
        h[i].dist = 0;
    }

    hash* re = (hash*)malloc(sizeof(hash));
    if (re == NULL)
    {
        free(h);
        return NULL;
    }

    re->list = h;
    re->func = (fun == NULL) ? default_hash_function : fun;
    re->capacity = c;
    return re;
}

int default_hash_function(int key)
{
    if (capa <= 0)
        return 0;

    int value = key % capa;
    return value < 0 ? value + capa : value;
}

void insert(hash* H, int value)
{
    if (H == NULL || H->list == NULL)
        return;

    if (H->func == default_hash_function)
        capa = H->capacity;

    int pos = H->func(value);
    if (pos < 0 || pos >= H->capacity)
        return;

    hashnode curr;
    curr.value = value;
    curr.dist = 0;
    curr.state = 1;

    for (int i = 0; i < H->capacity; i++)
    {
        int idx = (pos + curr.dist) % H->capacity;

        if (H->list[idx].state == 0)
        {
            H->list[idx] = curr;
            return;
        }

        if (H->list[idx].value == curr.value)
            return;

        if (H->list[idx].dist < curr.dist)
        {
            hashnode temp = H->list[idx];
            H->list[idx] = curr;
            curr = temp;
        }

        curr.dist++;
    }
}

int search(hash* H, int value)
{
    if (H == NULL || H->list == NULL)
        return -1;

    if (H->func == default_hash_function)
        capa = H->capacity;

    int pos = H->func(value);
    if (pos < 0 || pos >= H->capacity)
        return -1;

    for (int dist = 0; dist < H->capacity; dist++)
    {
        int idx = (pos + dist) % H->capacity;

        if (H->list[idx].state == 0)
            return -1;

        if (H->list[idx].dist < dist)
            return -1;

        if (H->list[idx].value == value)
            return idx;
    }
    return -1;
}

void delete_node(hash* H, int value)
{
    if (H == NULL || H->list == NULL)
        return;

    int idx = search(H, value);
    if (idx < 0)
        return;

    int next = (idx + 1) % H->capacity;
    while (H->list[next].state == 1 && H->list[next].dist > 0)
    {
        H->list[idx] = H->list[next];
        H->list[idx].dist--;
        idx = next;
        next = (next + 1) % H->capacity;
    }

    H->list[idx].state = 0;
    H->list[idx].dist = 0;
}

void freeH(hash* H)
{
    if (H == NULL)
        return;

    free(H->list);
    free(H);
}
