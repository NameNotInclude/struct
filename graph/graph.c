#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "graph.h"

/* ---------------- 建图 ---------------- */

Graph* graph_create(int nv, int directed)
{
    if (nv <= 0)
        return NULL;
    Graph *g = (Graph*)malloc(sizeof(Graph));
    g->adj = (GEdge**)calloc(nv, sizeof(GEdge*));
    g->nv = nv;
    g->ne = 0;
    g->directed = directed;
    return g;
}

int graph_add_edge(Graph *g, int from, int to, int weight)
{
    if (!g || from < 1 || from > g->nv || to < 1 || to > g->nv || from == to)
        return -1;

    GEdge *e = (GEdge*)malloc(sizeof(GEdge));
    e->to = to;
    e->weight = weight;
    e->next = g->adj[from - 1];
    g->adj[from - 1] = e;

    if (!g->directed)
    {
        GEdge *r = (GEdge*)malloc(sizeof(GEdge));
        r->to = from;
        r->weight = weight;
        r->next = g->adj[to - 1];
        g->adj[to - 1] = r;
    }

    g->ne++;
    return 0;
}

void graph_free(Graph *g)
{
    if (!g)
        return;
    for (int i = 0; i < g->nv; i++)
    {
        GEdge *e = g->adj[i];
        while (e)
        {
            GEdge *tmp = e->next;
            free(e);
            e = tmp;
        }
    }
    free(g->adj);
    free(g);
}

/* ---------------- Dijkstra（最小堆优化） ---------------- */

typedef struct {
    int *v;   /* 顶点 */
    int *d;   /* 距离 */
    int size;
    int cap;
} PHeap;

static void ph_swap(PHeap *h, int a, int b)
{
    int tv = h->v[a]; h->v[a] = h->v[b]; h->v[b] = tv;
    int td = h->d[a]; h->d[a] = h->d[b]; h->d[b] = td;
}

static void ph_push(PHeap *h, int v, int d)
{
    h->v[++h->size] = v;
    h->d[h->size] = d;
    int p = h->size;
    while (p > 1 && h->d[p] < h->d[p / 2])
    {
        ph_swap(h, p, p / 2);
        p = p / 2;
    }
}

static void ph_pop(PHeap *h, int *v, int *d)
{
    *v = h->v[1];
    *d = h->d[1];
    h->v[1] = h->v[h->size];
    h->d[1] = h->d[h->size];
    h->size--;

    int p = 1;
    while (p * 2 <= h->size)
    {
        int child = p * 2;
        if (child + 1 <= h->size && h->d[child + 1] < h->d[child])
            child++;
        if (h->d[p] <= h->d[child])
            break;
        ph_swap(h, p, child);
        p = child;
    }
}

int* graph_dijkstra(Graph *g, int source)
{
    if (!g || source < 1 || source > g->nv)
        return NULL;

    int n = g->nv;
    int *dist = (int*)malloc(sizeof(int) * n);
    int *visited = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX;
    dist[source - 1] = 0;

    int cap = 2 * g->ne + 2;
    if (cap < n + 1)
        cap = n + 1;
    PHeap h;
    h.v = (int*)malloc(sizeof(int) * (cap + 1));
    h.d = (int*)malloc(sizeof(int) * (cap + 1));
    h.size = 0;
    h.cap = cap;

    ph_push(&h, source, 0);

    while (h.size > 0)
    {
        int u, du;
        ph_pop(&h, &u, &du);
        if (visited[u - 1])
            continue;
        visited[u - 1] = 1;
        dist[u - 1] = du;

        for (GEdge *e = g->adj[u - 1]; e; e = e->next)
        {
            int w = e->to;
            if (!visited[w - 1] && du + e->weight < dist[w - 1])
            {
                dist[w - 1] = du + e->weight;
                ph_push(&h, w, dist[w - 1]);
            }
        }
    }

    free(h.v);
    free(h.d);
    free(visited);
    return dist;
}

/* ---------------- 最小生成树 ---------------- */

int graph_prim(Graph *g, int source)
{
    if (!g || source < 1 || source > g->nv)
        return -1;

    int n = g->nv;
    int *key = (int*)malloc(sizeof(int) * n);
    int *inMST = (int*)calloc(n, sizeof(int));
    for (int i = 0; i < n; i++)
        key[i] = INT_MAX;
    key[source - 1] = 0;

    int total = 0, cnt = 0;
    for (int i = 0; i < n; i++)
    {
        int u = -1, best = INT_MAX;
        for (int v = 0; v < n; v++)
        {
            if (!inMST[v] && key[v] < best)
            {
                best = key[v];
                u = v;
            }
        }
        if (u == -1)
            break;

        inMST[u] = 1;
        total += key[u];
        cnt++;

        for (GEdge *e = g->adj[u]; e; e = e->next)
        {
            int w = e->to - 1;
            if (!inMST[w] && e->weight < key[w])
                key[w] = e->weight;
        }
    }

    free(key);
    free(inMST);
    return (cnt == n) ? total : -1;
}

typedef struct {
    int from, to, weight;
} Edge;

static int edge_cmp(const void *a, const void *b)
{
    const Edge *ea = (const Edge*)a;
    const Edge *eb = (const Edge*)b;
    return ea->weight - eb->weight;
}

static int uf_find(int *parent, int x)
{
    if (parent[x] < 0)
        return x;
    return parent[x] = uf_find(parent, parent[x]);
}

static void uf_union(int *parent, int a, int b)
{
    int ra = uf_find(parent, a);
    int rb = uf_find(parent, b);
    if (ra == rb)
        return;
    if (parent[ra] > parent[rb])
    {
        parent[rb] += parent[ra];
        parent[ra] = rb;
    }
    else
    {
        parent[ra] += parent[rb];
        parent[rb] = ra;
    }
}

int graph_kruskal(Graph *g)
{
    if (!g)
        return -1;

    int n = g->nv;
    int maxE = g->ne > 0 ? g->ne : 1;
    Edge *edges = (Edge*)malloc(sizeof(Edge) * maxE);
    int m = 0;

    for (int u = 0; u < n; u++)
    {
        for (GEdge *e = g->adj[u]; e; e = e->next)
        {
            int v = e->to - 1;
            /* 无向图每条边存了两条弧，只保留 from < to 的一条 */
            if (!g->directed && u >= v)
                continue;
            edges[m].from = u + 1;
            edges[m].to = v + 1;
            edges[m].weight = e->weight;
            m++;
        }
    }

    qsort(edges, m, sizeof(Edge), edge_cmp);

    int *parent = (int*)malloc(sizeof(int) * (n + 1));
    for (int i = 1; i <= n; i++)
        parent[i] = -1;

    int total = 0, cnt = 0;
    for (int i = 0; i < m && cnt < n - 1; i++)
    {
        int rf = uf_find(parent, edges[i].from);
        int rt = uf_find(parent, edges[i].to);
        if (rf != rt)
        {
            uf_union(parent, rf, rt);
            total += edges[i].weight;
            cnt++;
        }
    }

    free(edges);
    free(parent);
    return (cnt == n - 1) ? total : -1;
}

/* ---------------- 拓扑排序 ---------------- */

int graph_toposort(Graph *g, int *order)
{
    if (!g || !order)
        return -1;

    int n = g->nv;
    int *indeg = (int*)calloc(n, sizeof(int));
    for (int u = 0; u < n; u++)
        for (GEdge *e = g->adj[u]; e; e = e->next)
            indeg[e->to - 1]++;

    int *stack = (int*)malloc(sizeof(int) * n);
    int top = 0;
    for (int i = 0; i < n; i++)
        if (indeg[i] == 0)
            stack[top++] = i + 1;

    int cnt = 0;
    while (top > 0)
    {
        int u = stack[--top];
        order[cnt++] = u;
        for (GEdge *e = g->adj[u - 1]; e; e = e->next)
        {
            int w = e->to - 1;
            if (--indeg[w] == 0)
                stack[top++] = w + 1;
        }
    }

    free(indeg);
    free(stack);
    return (cnt == n) ? cnt : -1;
}

/* ---------------- Tarjan 强连通分量 ---------------- */

static int *t_idx, *t_low, *t_onstack, *t_stack;
static int t_counter, t_top, t_scc_count;

static void tarjan_dfs(Graph *g, int v)
{
    t_idx[v] = t_low[v] = t_counter++;
    t_stack[t_top++] = v;
    t_onstack[v] = 1;

    for (GEdge *e = g->adj[v - 1]; e; e = e->next)
    {
        int w = e->to;
        if (t_idx[w] == -1)
        {
            tarjan_dfs(g, w);
            if (t_low[w] < t_low[v])
                t_low[v] = t_low[w];
        }
        else if (t_onstack[w])
        {
            if (t_idx[w] < t_low[v])
                t_low[v] = t_idx[w];
        }
    }

    if (t_low[v] == t_idx[v])
    {
        int w;
        do
        {
            w = t_stack[--t_top];
            t_onstack[w] = 0;
            printf("%d ", w);
        } while (w != v);
        printf("\n");
        t_scc_count++;
    }
}

int graph_scc(Graph *g)
{
    if (!g)
        return 0;

    int n = g->nv;
    t_idx = (int*)malloc(sizeof(int) * (n + 1));
    t_low = (int*)malloc(sizeof(int) * (n + 1));
    t_onstack = (int*)malloc(sizeof(int) * (n + 1));
    t_stack = (int*)malloc(sizeof(int) * (n + 1));
    t_counter = 0;
    t_top = 0;
    t_scc_count = 0;

    for (int i = 1; i <= n; i++)
    {
        t_idx[i] = -1;
        t_onstack[i] = 0;
    }

    for (int i = 1; i <= n; i++)
        if (t_idx[i] == -1)
            tarjan_dfs(g, i);

    free(t_idx);
    free(t_low);
    free(t_onstack);
    free(t_stack);
    return t_scc_count;
}

/* ---------------- AOE 关键路径 ---------------- */

int graph_critical_path(Graph *g, int source, int sink)
{
    if (!g || source < 1 || source > g->nv || sink < 1 || sink > g->nv)
        return -1;

    int n = g->nv;

    /* 求入度 */
    int *indeg = (int*)calloc(n + 1, sizeof(int));
    for (int u = 0; u < n; u++)
        for (GEdge *e = g->adj[u]; e; e = e->next)
            indeg[e->to]++;

    /* 拓扑排序 */
    int *topo = (int*)malloc(sizeof(int) * (n + 1));
    int *stack = (int*)malloc(sizeof(int) * (n + 1));
    int top = 0, cnt = 0;
    for (int i = 1; i <= n; i++)
        if (indeg[i] == 0)
            stack[top++] = i;

    while (top > 0)
    {
        int u = stack[--top];
        topo[cnt++] = u;
        for (GEdge *e = g->adj[u - 1]; e; e = e->next)
        {
            int w = e->to;
            if (--indeg[w] == 0)
                stack[top++] = w;
        }
    }

    if (cnt != n)
    {
        printf("Error: Graph has a cycle!\n");
        free(indeg);
        free(topo);
        free(stack);
        return -1;
    }

    /* 最早发生时间 EC */
    int *EC = (int*)malloc(sizeof(int) * (n + 1));
    int *LC = (int*)malloc(sizeof(int) * (n + 1));
    for (int i = 1; i <= n; i++)
    {
        EC[i] = INT_MIN;
        LC[i] = INT_MAX;
    }
    EC[source] = 0;

    for (int i = 0; i < cnt; i++)
    {
        int u = topo[i];
        if (EC[u] == INT_MIN)
            continue;
        for (GEdge *e = g->adj[u - 1]; e; e = e->next)
        {
            int w = e->to;
            if (EC[u] + e->weight > EC[w])
                EC[w] = EC[u] + e->weight;
        }
    }

    if (EC[sink] == INT_MIN)
    {
        printf("Error: No path from %d to %d!\n", source, sink);
        free(indeg);
        free(topo);
        free(stack);
        free(EC);
        free(LC);
        return -1;
    }

    /* 最晚发生时间 LC（逆拓扑序） */
    LC[sink] = EC[sink];
    for (int i = cnt - 1; i >= 0; i--)
    {
        int v = topo[i];
        for (GEdge *e = g->adj[v - 1]; e; e = e->next)
        {
            int w = e->to;
            if (LC[w] != INT_MAX && LC[w] - e->weight < LC[v])
                LC[v] = LC[w] - e->weight;
        }
    }

    /* 打印关键活动 */
    printf("Critical activities:\n");
    for (int u = 1; u <= n; u++)
    {
        for (GEdge *e = g->adj[u - 1]; e; e = e->next)
        {
            int w = e->to;
            if (EC[u] != INT_MIN && LC[w] != INT_MAX)
            {
                int earliest = EC[u];
                int latest = LC[w] - e->weight;
                if (earliest == latest)
                    printf("%d -> %d (weight=%d)\n", u, w, e->weight);
            }
        }
    }

    free(indeg);
    free(topo);
    free(stack);
    free(EC);
    free(LC);
    return 0;
}
