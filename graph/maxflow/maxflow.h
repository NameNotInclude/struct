#ifndef MAXFLOW_H
#define MAXFLOW_H

/*
 * 最大流：邻接矩阵表示（容量网络），顶点编号从 1 开始。
 * 支持 Edmonds-Karp 与 Dinic 两种算法。
 */
typedef struct {
    int **cap;  /* cap[i][j] 为 i+1 -> j+1 的容量 */
    int nv;
} FlowGraph;

FlowGraph* flow_create(int nv);

/* 添加一条有向边（容量叠加）；失败返回 -1 */
int flow_add_edge(FlowGraph *g, int from, int to, int cap);

void flow_free(FlowGraph *g);

/* 计算 s 到 t 的最大流 */
int flow_edmonds_karp(FlowGraph *g, int s, int t);
int flow_dinic(FlowGraph *g, int s, int t);

#endif /* MAXFLOW_H */
