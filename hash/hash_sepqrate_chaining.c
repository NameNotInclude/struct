#include <stdio.h>
#include <stdlib.h>

typedef struct H
{
    int value;
    struct H* next;
} Hnode;

typedef Hnode* PtrToNode;

typedef int (*hashf)(int);

typedef struct
{
    int capacity;
    PtrToNode* list;
    hashf func;
} hash;

int capa;

hash* creat(int c, hashf fun);
int default_hash_function(int key);
void insert(hash* H, int value);
void delete_node(hash* H, int value);
PtrToNode search(hash* H, int value);
void freeH(hash* h);

int main()
{
    return 0;
}

hash* creat(int c, hashf fun)
{
    if (c <= 0)
        return NULL;

    PtrToNode* h = (PtrToNode*)malloc(sizeof(PtrToNode) * c);
    if (h == NULL)
        return NULL;

    for (int i = 0; i < c; i++)
        h[i] = NULL;

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

    PtrToNode newnode = (PtrToNode)malloc(sizeof(Hnode));
    if (newnode == NULL)
        return;

    newnode->value = value;
    newnode->next = H->list[pos];
    H->list[pos] = newnode;
}

PtrToNode search(hash* H, int value)
{
    if (H == NULL || H->list == NULL)
        return NULL;

    if (H->func == default_hash_function)
        capa = H->capacity;

    int pos = H->func(value);
    if (pos < 0 || pos >= H->capacity)
        return NULL;

    PtrToNode curr = H->list[pos];
    while (curr != NULL)
    {
        if (curr->value == value)
            return curr;
        curr = curr->next;
    }
    return NULL;
}

void delete_node(hash* H, int value)
{
    if (H == NULL || H->list == NULL)
        return;

    if (H->func == default_hash_function)
        capa = H->capacity;

    int pos = H->func(value);
    if (pos < 0 || pos >= H->capacity)
        return;

    PtrToNode prev = NULL;
    PtrToNode curr = H->list[pos];
    while (curr != NULL)
    {
        if (curr->value == value)
        {
            if (prev == NULL)
                H->list[pos] = curr->next;
            else
                prev->next = curr->next;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

void freeH(hash* H)
{
    if (H == NULL)
        return;

    for (int i = 0; i < H->capacity; i++)
    {
        PtrToNode curr = H->list[i];
        while (curr != NULL)
        {
            PtrToNode temp = curr->next;
            free(curr);
            curr = temp;
        }
    }
    free(H->list);
    free(H);
}

