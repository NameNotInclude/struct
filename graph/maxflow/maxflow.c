#include <stdlib.h>
#include <limits.h>
#include "../../stack_queue/stack_queue.h"
#include "maxflow.h"

FlowGraph* flow_create(int nv)
{
    if (nv <= 0)
        return NULL;
    FlowGraph *g = (FlowGraph*)malloc(sizeof(FlowGraph));
    g->cap = (int**)malloc(sizeof(int*) * nv);
    for (int i = 0; i < nv; i++)
        g->cap[i] = (int*)calloc(nv, sizeof(int));
    g->nv = nv;
    return g;
}

int flow_add_edge(FlowGraph *g, int from, int to, int cap)
{
    if (!g || from < 1 || from > g->nv || to < 1 || to > g->nv ||
        from == to || cap < 0)
        return -1;
    g->cap[from - 1][to - 1] += cap;
    return 0;
}

void flow_free(FlowGraph *g)
{
    if (!g)
        return;
    for (int i = 0; i < g->nv; i++)
        free(g->cap[i]);
    free(g->cap);
    free(g);
}

static FlowGraph* flow_copy(FlowGraph *g)
{
    FlowGraph *c = flow_create(g->nv);
    for (int i = 0; i < g->nv; i++)
        for (int j = 0; j < g->nv; j++)
            c->cap[i][j] = g->cap[i][j];
    return c;
}

/* BFS 在残余网络中找增广路径，parent 记录路径 */
static int bfs(FlowGraph *g, int s, int t, int *parent)
{
    queue *q = creatQ(g->nv);
    int *visited = (int*)calloc(g->nv + 1, sizeof(int));

    visited[s] = 1;
    parent[s] = -1;
    enqueue(q, s);

    while (q->size > 0)
    {
        int u = dequeue(q);
        for (int v = 1; v <= g->nv; v++)
        {
            if (!visited[v] && g->cap[u - 1][v - 1] > 0)
            {
                visited[v] = 1;
                parent[v] = u;
                if (v == t)
                {
                    free(visited);
                    freeQ(q);
                    return 1;
                }
                enqueue(q, v);
            }
        }
    }

    free(visited);
    freeQ(q);
    return 0;
}

int flow_edmonds_karp(FlowGraph *g, int s, int t)
{
    if (!g || s < 1 || s > g->nv || t < 1 || t > g->nv)
        return -1;

    FlowGraph *rest = flow_copy(g);
    int *parent = (int*)malloc(sizeof(int) * (g->nv + 1));
    int flow = 0;

    while (bfs(rest, s, t, parent))
    {
        int min = INT_MAX;
        for (int v = t; v != s; v = parent[v])
        {
            int u = parent[v];
            if (rest->cap[u - 1][v - 1] < min)
                min = rest->cap[u - 1][v - 1];
        }

        for (int v = t; v != s; v = parent[v])
        {
            int u = parent[v];
            rest->cap[u - 1][v - 1] -= min;
            rest->cap[v - 1][u - 1] += min;
        }
        flow += min;
    }

    free(parent);
    flow_free(rest);
    return flow;
}

/* ---------------- Dinic ---------------- */

static int dinic_bfs(FlowGraph *g, int s, int t, int *level)
{
    for (int i = 1; i <= g->nv; i++)
        level[i] = -1;

    queue *q = creatQ(g->nv);
    enqueue(q, s);
    level[s] = 0;

    while (q->size > 0)
    {
        int u = dequeue(q);
        for (int v = 1; v <= g->nv; v++)
        {
            if (level[v] < 0 && g->cap[u - 1][v - 1] > 0)
            {
                level[v] = level[u] + 1;
                enqueue(q, v);
            }
        }
    }

    freeQ(q);
    return level[t] >= 0;
}

static int dinic_dfs(FlowGraph *g, int u, int t, int flow, int *level, int *next)
{
    if (u == t)
        return flow;

    for (int v = next[u]; v <= g->nv; v++)
    {
        next[u] = v + 1; /* 当前弧优化 */
        if (level[v] == level[u] + 1 && g->cap[u - 1][v - 1] > 0)
        {
            int pushed = dinic_dfs(g, v, t,
                                   flow < g->cap[u - 1][v - 1] ? flow : g->cap[u - 1][v - 1],
                                   level, next);
            if (pushed > 0)
            {
                g->cap[u - 1][v - 1] -= pushed;
                g->cap[v - 1][u - 1] += pushed;
                return pushed;
            }
        }
    }
    return 0;
}

int flow_dinic(FlowGraph *g, int s, int t)
{
    if (!g || s < 1 || s > g->nv || t < 1 || t > g->nv)
        return -1;

    FlowGraph *rest = flow_copy(g);
    int *level = (int*)malloc(sizeof(int) * (g->nv + 1));
    int *next = (int*)malloc(sizeof(int) * (g->nv + 1));
    int flow = 0;

    while (dinic_bfs(rest, s, t, level))
    {
        for (int i = 1; i <= g->nv; i++)
            next[i] = 1;

        while (1)
        {
            int pushed = dinic_dfs(rest, s, t, INT_MAX, level, next);
            if (pushed == 0)
                break;
            flow += pushed;
        }
    }

    free(level);
    free(next);
    flow_free(rest);
    return flow;
}
