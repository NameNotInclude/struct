#include <stdio.h>
#include <stdlib.h>

typedef int (*hashf)(int);

typedef struct 
{
    int value;
    short state;
    //0:empty,1:normal,-1:deleted
}hashnode;

typedef struct 
{
    hashnode* list;
    int capacity;
    hashf func;
}hash;

int capa;
//0:linear,1:quadratic,2:double hash
int mode;
hashf hash2=NULL;

hash* creat(int c, hashf fun);
int default_hash_function(int key);
void insert(hash* f, int a);
void delete_node(hash* f, int a);
int search(hash* f, int a);
void clear(hash* f);

int main()
{
    hash* h=creat(23,NULL);
    for (int i=0;i<7;i++)
    {
        int x;
        scanf("%d",&x);
        insert(h,x);
    }

    for (int i=0;i<23;i++)
    {
        printf("%4d ",i);
    }
    printf("\n");
    for (int i=0;i<23;i++)
    {
        if(h->list[i].state==1)
            printf("%d ",h->list[i].value);

        else
            printf("  -1 ");
    }
    
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
        h[i].state = 0;

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
static int get_probe_index(hash* f, int a, int i)
{
    int pos = f->func(a);
    switch (mode)
    {
        case 0:
            return (pos + i) % f->capacity;
        case 1:
            return (pos + i * i) % f->capacity;
        case 2:
        {
            if (hash2 == NULL)
                return -1;
            int d = hash2(a);
            if (d % f->capacity == 0)
                d = 1;
            return (pos + i * d) % f->capacity;
        }
        default:
            return -1;
    }
}

void insert(hash* f, int a)
{
    if (f == NULL || f->list == NULL)
        return;

    if (f->func == default_hash_function)
        capa = f->capacity;

    int first_deleted = -1;
    for (int i = 0; i < f->capacity; i++)
    {
        int idx = get_probe_index(f, a, i);
        if (idx < 0)
            return;

        if (f->list[idx].state == 0)
        {
            int target = (first_deleted >= 0) ? first_deleted : idx;
            f->list[target].value = a;
            f->list[target].state = 1;
            return;
        }

        if (f->list[idx].state == -1 && first_deleted < 0)
            first_deleted = idx;

        if (f->list[idx].state == 1 && f->list[idx].value == a)
            return;
    }

    if (first_deleted >= 0)
    {
        f->list[first_deleted].value = a;
        f->list[first_deleted].state = 1;
    }
}
int search(hash* f, int a)
{
    if (f == NULL || f->list == NULL)
        return -1;

    if (f->func == default_hash_function)
        capa = f->capacity;

    for (int i = 0; i < f->capacity; i++)
    {
        int idx = get_probe_index(f, a, i);
        if (idx < 0)
            return -1;

        if (f->list[idx].state == 0)
            return -1;

        if (f->list[idx].state == 1 && f->list[idx].value == a)
            return idx;
    }
    return -1;
}

void delete_node(hash* f, int a)
{
    if (f == NULL || f->list == NULL)
        return;

    if (f->func == default_hash_function)
        capa = f->capacity;

    for (int i = 0; i < f->capacity; i++)
    {
        int idx = get_probe_index(f, a, i);
        if (idx < 0)
            return;

        if (f->list[idx].state == 0)
            break;

        if (f->list[idx].state == 1 && f->list[idx].value == a)
        {
            f->list[idx].state = -1;
            return;
        }
    }
}
void clear(hash* f)
{
    if (f == NULL || f->list == NULL)
        return;

    for (int i = 0; i < f->capacity; i++)
        if (f->list[i].state == -1)
            f->list[i].state = 0;
}
void freeH(hash* H)
{
    free(H->list);
    free(H);
}

