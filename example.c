/*
 * example.c —— 综合示例：演示如何调用本项目中的各数据结构库。
 *
 * 编译（见 Makefile）：
 *   gcc -std=c99 -Wall -Wextra -I. -o example example.c \
 *       linkedlist/linkedlist.c stack_queue/stack_queue.c \
 *       heap/heap.c hash/hash_open_address.c hash/hash_robinhood.c \
 *       hash/hash_sepqrate_chaining.c disjointset/disjointset.c \
 *       tree/binarytree.c tree/zigzaggingtree.c \
 *       graph/graph.c graph/maxflow/maxflow.c -lm
 */
#include <stdio.h>
#include <stdlib.h>

#include "linkedlist/linkedlist.h"
#include "stack_queue/stack_queue.h"
#include "heap/heap.h"
#include "hash/hash.h"
#include "disjointset/disjointset.h"
#include "tree/binarytree.h"
#include "tree/zigzaggingtree.h"
#include "graph/graph.h"
#include "graph/maxflow/maxflow.h"

static void print_int(int v)
{
    printf("%d ", v);
}

int main(void)
{
    /* 1. 链表 */
    printf("== Linked List ==\n");
    List list = list_create();
    list_append(&list, 10);
    list_append(&list, 20);
    list_insert(&list, 5);
    printf("size=%d, find(20)=%d, iterate: ", list_size(&list), list_find(&list, 20));
    list_iterate(&list, print_int);
    printf("\n");
    list_remove(&list, 20);
    printf("after remove(20): ");
    list_iterate(&list, print_int);
    printf("\n");
    list_free(&list);

    /* 2. 栈与队列 */
    printf("\n== Stack & Queue ==\n");
    stack *s = creatS(10);
    push(s, 1);
    push(s, 2);
    push(s, 3);
    int s1 = pop(s);
    int s2 = pop(s);
    printf("stack pop: %d %d\n", s1, s2);
    queue *q = creatQ(10);
    enqueue(q, 7);
    enqueue(q, 8);
    enqueue(q, 9);
    int d1 = dequeue(q);
    int d2 = dequeue(q);
    printf("queue dequeue: %d %d\n", d1, d2);
    freeS(s);
    freeQ(q);

    /* 3. 堆 */
    printf("\n== Heap ==\n");
    int arr[] = {43, 12, 5, 78, 33, 91};
    Heap *h = heap_create(10);
    heap_build(h, arr, 6);
    int cur_min = heap_min(h);
    int del_min = heap_delete_min(h);
    printf("min=%d, delete-min=%d\n", cur_min, del_min);
    heap_insert(h, 1);
    printf("after insert 1, min=%d\n", heap_min(h));
    heap_free(h);

    int sort_arr[] = {5, 2, 9, 1, 7};
    heap_sort(sort_arr, 5);
    printf("heap_sort: ");
    for (int i = 0; i < 5; i++)
        printf("%d ", sort_arr[i]);
    printf("\n");

    /* 4. 哈希表（三种冲突解决策略） */
    printf("\n== Hash Table ==\n");
    CHHash *ch = ch_create(11);
    ch_insert(ch, 15);
    ch_insert(ch, 26);
    ch_insert(ch, 37);
    printf("chaining: search(26)=%d search(99)=%d\n", ch_search(ch, 26), ch_search(ch, 99));
    ch_delete(ch, 26);
    printf("after delete(26): search(26)=%d\n", ch_search(ch, 26));
    ch_free(ch);

    OAHash *oa = oa_create(11, 0); /* 线性探测 */
    oa_insert(oa, 15);
    oa_insert(oa, 26);
    printf("open addressing(linear): search(26)=%d\n", oa_search(oa, 26));
    oa_free(oa);

    RHHash *rh = rh_create(11);
    rh_insert(rh, 15);
    rh_insert(rh, 26);
    printf("robin hood: search(26)=%d\n", rh_search(rh, 26));
    rh_free(rh);

    /* 5. 并查集 */
    printf("\n== Disjoint Set ==\n");
    DisjointSet *d = disjointset_create(6);
    disjointset_union(d, 1, 2);
    disjointset_union(d, 3, 4);
    disjointset_union(d, 1, 3);
    printf("connected(1,4)=%d connected(1,5)=%d\n",
           disjointset_connected(d, 1, 4), disjointset_connected(d, 1, 5));
    disjointset_free(d);

    /* 6. 二叉搜索树 */
    printf("\n== Binary Search Tree ==\n");
    int a[] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    BTNode *root = bt_build(a, 10);
    printf("inorder: ");
    bt_inorder(root);
    printf("\n");
    printf("preorder(iter): ");
    bt_preorder_iter(root);
    printf("\n");
    printf("levelorder: ");
    bt_levelorder(root);
    printf("\n");
    bt_insert(root, 11);
    printf("after insert 11, inorder: ");
    bt_inorder(root);
    printf("\n");
    bt_delete(root, 5);
    printf("after delete 5, inorder: ");
    bt_inorder(root);
    printf("\n");
    printf("depth=%d, find(8)=%s\n", bt_depth(root), bt_find(root, 8) ? "found" : "not found");
    bt_free(root);

    /* 7. 之字形树（中序 + 后序重建） */
    printf("\n== Zigzag Tree ==\n");
    int inorder[] = {12, 11, 20, 17, 1, 15, 8, 5};
    int postorder[] = {12, 20, 17, 11, 15, 8, 5, 1};
    ZNode *z = zigzag_build(inorder, postorder, 8);
    printf("zigzag: ");
    zigzag_print(z);
    zigzag_free(z);

    /* 8. 图论 */
    printf("\n== Graph ==\n");
    /* 无向带权图 */
    Graph *g = graph_create(5, 0);
    graph_add_edge(g, 1, 2, 3);
    graph_add_edge(g, 1, 3, 1);
    graph_add_edge(g, 2, 3, 7);
    graph_add_edge(g, 2, 4, 5);
    graph_add_edge(g, 3, 4, 4);
    graph_add_edge(g, 4, 5, 2);
    int *dist = graph_dijkstra(g, 1);
    printf("dijkstra from 1: ");
    for (int i = 0; i < 5; i++)
        printf("%d ", dist[i]);
    printf("\n");
    free(dist);
    printf("prim total=%d, kruskal total=%d\n", graph_prim(g, 1), graph_kruskal(g));
    graph_free(g);

    /* 有向图：拓扑排序 + 强连通分量 */
    Graph *dg = graph_create(6, 1);
    graph_add_edge(dg, 1, 2, 1);
    graph_add_edge(dg, 2, 3, 1);
    graph_add_edge(dg, 3, 1, 1); /* 1-2-3 成环 */
    graph_add_edge(dg, 4, 5, 1);
    graph_add_edge(dg, 5, 6, 1);
    int order[6];
    int cnt = graph_toposort(dg, order);
    printf("toposort: %s\n", cnt < 0 ? "(has cycle)" : "order:");
    if (cnt > 0)
    {
        for (int i = 0; i < cnt; i++)
            printf("%d ", order[i]);
        printf("\n");
    }
    printf("strongly connected components:\n");
    int scc_count = graph_scc(dg);
    printf("total SCC = %d\n", scc_count);
    graph_free(dg);

    /* AOE 关键路径 */
    Graph *aoe = graph_create(6, 1);
    graph_add_edge(aoe, 1, 2, 3);
    graph_add_edge(aoe, 1, 3, 2);
    graph_add_edge(aoe, 2, 4, 2);
    graph_add_edge(aoe, 2, 5, 3);
    graph_add_edge(aoe, 3, 4, 4);
    graph_add_edge(aoe, 4, 6, 2);
    graph_add_edge(aoe, 5, 6, 1);
    graph_critical_path(aoe, 1, 6);
    graph_free(aoe);

    /* 9. 最大流 */
    printf("\n== Max Flow ==\n");
    FlowGraph *fg = flow_create(4);
    flow_add_edge(fg, 1, 2, 3);
    flow_add_edge(fg, 1, 3, 2);
    flow_add_edge(fg, 2, 3, 1);
    flow_add_edge(fg, 2, 4, 2);
    flow_add_edge(fg, 3, 4, 3);
    printf("Edmonds-Karp maxflow=%d\n", flow_edmonds_karp(fg, 1, 4));
    printf("Dinic maxflow=%d\n", flow_dinic(fg, 1, 4));
    flow_free(fg);

    return 0;
}
