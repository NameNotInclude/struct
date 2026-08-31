#ifndef DISJOINTSET_H
#define DISJOINTSET_H

/*
 * 并查集（Union-Find）：按大小合并 + 路径压缩。
 * 元素编号从 1 到 n。
 */
typedef struct {
    int *parent; /* parent[i] < 0 表示 i 是根，其绝对值是集合大小 */
    int n;
} DisjointSet;

DisjointSet* disjointset_create(int n);
void disjointset_free(DisjointSet *d);

/* 返回 x 所在集合的根（带路径压缩） */
int disjointset_find(DisjointSet *d, int x);

/* 合并 a、b 所在集合（按大小合并） */
void disjointset_union(DisjointSet *d, int a, int b);

/* 判断 a、b 是否连通 */
int disjointset_connected(DisjointSet *d, int a, int b);

#endif /* DISJOINTSET_H */
