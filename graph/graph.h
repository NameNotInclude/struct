#ifndef GRAPH_H
#define GRAPH_H

/*
 * 统一的图库：邻接表实现（带权），顶点编号从 1 开始。
 * 支持有向图 / 无向图，以及常见图论算法。
 */
typedef struct GEdge {
    int to;             /* 1 起始的顶点编号 */
    int weight;
    struct GEdge *next;
} GEdge;

typedef struct {
    GEdge **adj;        /* adj[i] 是第 i+1 个顶点的出边链表 */
    int nv;             /* 顶点数 */
    int ne;             /* 边数 */
    int directed;       /* 1 有向, 0 无向 */
} Graph;

/* 创建图：nv 个顶点，directed 指定是否有向 */
Graph* graph_create(int nv, int directed);

/* 添加一条边（无向图会自动加反向边）；失败返回 -1 */
int graph_add_edge(Graph *g, int from, int to, int weight);

void graph_free(Graph *g);

/* Dijkstra 单源最短路径：返回长度 nv 的 dist 数组（下标 0 起始），调用者释放 */
int* graph_dijkstra(Graph *g, int source);

/* 最小生成树（无向图）：返回总权重，图不连通返回 -1 */
int graph_prim(Graph *g, int source);
int graph_kruskal(Graph *g);

/* 拓扑排序：结果写入 order（1 起始编号），返回顶点数；有环返回 -1 */
int graph_toposort(Graph *g, int *order);

/* Tarjan 强连通分量：打印各分量，返回分量个数 */
int graph_scc(Graph *g);

/* AOE 关键路径：打印关键活动，成功返回 0，失败返回 -1 */
int graph_critical_path(Graph *g, int source, int sink);

#endif /* GRAPH_H */
