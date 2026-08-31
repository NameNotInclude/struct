#include <stdlib.h>
#include "disjointset.h"

DisjointSet* disjointset_create(int n)
{
    DisjointSet *d = (DisjointSet*)malloc(sizeof(DisjointSet));
    d->parent = (int*)malloc(sizeof(int) * (n + 1));
    d->n = n;
    for (int i = 1; i <= n; i++)
        d->parent[i] = -1;
    return d;
}

void disjointset_free(DisjointSet *d)
{
    if (!d)
        return;
    free(d->parent);
    free(d);
}

int disjointset_find(DisjointSet *d, int x)
{
    if (d->parent[x] < 0)
        return x;
    return d->parent[x] = disjointset_find(d, d->parent[x]);
}

void disjointset_union(DisjointSet *d, int a, int b)
{
    int ra = disjointset_find(d, a);
    int rb = disjointset_find(d, b);
    if (ra == rb)
        return;

    /* parent 越负表示集合越大；把小的挂到大的下面 */
    if (d->parent[ra] > d->parent[rb])
    {
        d->parent[rb] += d->parent[ra];
        d->parent[ra] = rb;
    }
    else
    {
        d->parent[ra] += d->parent[rb];
        d->parent[rb] = ra;
    }
}

int disjointset_connected(DisjointSet *d, int a, int b)
{
    return disjointset_find(d, a) == disjointset_find(d, b);
}
