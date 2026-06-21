#include <stdio.h>
#include <stdlib.h>

#define MAX 2147483647

typedef struct 
{
    int* q;
    int front;
    int rear;
    int size;
    int capacity;
}queue;

typedef struct 
{
    int** G;
    int Nv;
    int Ne;
}Graph;

queue* creatQ(int capacity);
void enqueue(queue* q,int tar);
int dequeue(queue* q);
int visitQ(queue* q);
void freeQ(queue* q);

Graph* creat();
void freeG(Graph* Gr);
int BFS(int s,int t,Graph* Gr,int parent[]);
int Edmonds_Karp(int s,int t,Graph* Gr);
int DinicBFS(int s,int t,Graph* Gr,int level[]);
int DinicDFS(int u,int t,int flow,Graph* Gr,int level[],int next[]);
int Dinic(int s,int t,Graph* Gr);

int main()
{
    Graph* flow=creat();
    printf("%d",Dinic(1,flow->Nv,flow));
    freeG(flow);
    return 0;
}

Graph* creat()
{
    Graph* Gr=(Graph*)malloc(sizeof(Graph));
    scanf("%d %d",&(Gr->Nv),&(Gr->Ne));

    int** a=(int**)malloc(sizeof(int*)*(Gr->Nv)); 
    for (int i=0;i<Gr->Nv;i++)
    {
        a[i]=(int*)malloc(sizeof(int)*(Gr->Nv));
        for (int j=0;j<Gr->Nv;j++)
            a[i][j]=0;
    }

    int i=0;
    while (i<Gr->Ne)
    {
        int from,to,weight;
        scanf("%d %d %d",&from,&to,&weight);

        if (from<1 || from>Gr->Nv || to<1 || to>Gr->Nv)
        {
            printf("Invalid input\n");
            continue;
        }

        if (a[from-1][to-1]!=0)
        {
            printf("Duplicated edge\n");
            continue;
        }

        a[from-1][to-1]=weight;
        i++;
    }
    Gr->G=a;

    return Gr;
}
void freeG(Graph* Gr)
{
    if (Gr==NULL) return;

    for (int i=0;i<Gr->Nv;i++)
        free(Gr->G[i]);

    free(Gr->G);

    free(Gr);
}
Graph* copy(Graph* Gr)
{
    Graph* co=(Graph*)malloc(sizeof(Graph));
    co->Ne=Gr->Ne;
    co->Nv=Gr->Nv;

    int** a=(int**)malloc(sizeof(int*)*(Gr->Nv)); 
    for (int i=0;i<Gr->Nv;i++)
    {
        a[i]=(int*)malloc(sizeof(int)*(Gr->Nv));
        for (int j=0;j<Gr->Nv;j++)
            a[i][j]=Gr->G[i][j];
    }

    co->G=a;

    return co;
}

int BFS(int s,int t,Graph* Gr,int parent[])
{
    queue* Q=creatQ(Gr->Nv);
    enqueue(Q,s);
    parent[s]=-1;
    int* visited=(int*)malloc(sizeof(int)*(Gr->Nv+1));
    for (int i=1;i<=Gr->Nv;i++)
        visited[i]=0;
    visited[s]=1;

    while (Q->size>0)
    {
        int u=dequeue(Q);

        //将所有与u相连的节点v加入队列
        for (int v=1;v<=Gr->Nv;v++)
        {
            if (!visited[v] && Gr->G[u-1][v-1])
            {
                visited[v]=1;
                parent[v]=u;
                //v=t，找到一条到终点的路径，返回路径
                if (v==t)
                {
                    free(visited);
                    freeQ(Q);
                    return 1;
                }
                enqueue(Q,v);
            }
        }
    }

    free(visited);
    freeQ(Q);
    return 0;
}
int Edmonds_Karp(int s,int t,Graph* Gr)
{
    int flow=0;
    int* p=(int*)malloc(sizeof(int)*(Gr->Nv+1));

    Graph* rest=copy(Gr);

    while (BFS(s,t,rest,p))
    {
        int min=MAX;

        //找到路径当中的最小流
        for (int v=t;v!=s;v=p[v])
        {
            if (rest->G[p[v]-1][v-1]<min)
                min=rest->G[p[v]-1][v-1];
        }

        //正向边减去流，反向边加上流
        for (int v=t;v!=s;v=p[v])
        {
            rest->G[p[v]-1][v-1]-=min;
            rest->G[v-1][p[v]-1]+=min;
        }
        flow+=min;
    }
    free(p);
    freeG(rest);
    return flow;
}

int DinicBFS(int s,int t,Graph* Gr,int level[])
{
    for (int i=1;i<=Gr->Nv;i++)
        level[i]=-1;

    queue* Q=creatQ(Gr->Nv);
    enqueue(Q,s);
    level[s]=0;

    while (Q->size>0)
    {
        int u=dequeue(Q);
        for (int v=1;v<=Gr->Nv;v++)
        {
            if (level[v] < 0 && Gr->G[u-1][v-1] > 0)
            {
                level[v] = level[u] + 1;
                enqueue(Q,v);
            }
        }
    }

    freeQ(Q);
    return level[t] >= 0;
}
int DinicDFS(int u,int t,int flow,Graph* Gr,int level[],int next[])
{
    if (u == t)
        return flow;

    //从next[u]开始尝试所有的节点
    for (int v = next[u]; v <= Gr->Nv; v++)
    {
        //弧优化：记录当前尝试的v，下一次从v+1开始
        next[u] = v + 1;

        //检查v是不是满足层序关系
        if (level[v] == level[u] + 1 && Gr->G[u-1][v-1] > 0)
        {
            //flow < Gr->G[u-1][v-1] ? flow : Gr->G[u-1][v-1]
            //上式：计算u到v后，路径上剩余的最小容量

            //向下递归探索
            int pushed = DinicDFS(v, t, flow < Gr->G[u-1][v-1] ? flow : Gr->G[u-1][v-1], Gr, level, next);
            
            //pushed>0，说明找到增广路径
            if (pushed > 0)
            {
                //正向边减少
                Gr->G[u-1][v-1] -= pushed;

                //反向边增加
                Gr->G[v-1][u-1] += pushed;
                return pushed;
            }
        }
    }

    return 0;
}
int Dinic(int s,int t,Graph* Gr)
{
    Graph* rest = copy(Gr);
    int flow = 0;
    int* level = (int*)malloc(sizeof(int) * (rest->Nv + 1));
    int* next = (int*)malloc(sizeof(int) * (rest->Nv + 1));

    while (DinicBFS(s, t, rest, level))
    {
        for (int i = 1; i <= rest->Nv; i++)
            next[i] = 1;

        while (1)
        {
            int pushed = DinicDFS(s, t, MAX, rest, level, next);
            if (pushed == 0)
                break;
            flow += pushed;
        }
    }

    free(level);
    free(next);
    freeG(rest);
    return flow;
}

queue* creatQ(int capacity)
{
    queue* S=(queue*)malloc(sizeof(queue));
    S->capacity=capacity;
    S->size=0;
    S->front=0;
    S->rear=0;

    int* H=(int*)malloc(sizeof(int)*capacity);
    S->q=H;

    return S;
}
void enqueue(queue* q,int tar)
{
    if (q->size==q->capacity)
    return;

    q->q[q->rear]=tar;
    
    q->size++;
    q->rear=(q->rear+1)%q->capacity;
}
int dequeue(queue* q)
{
    if (q->size==0)
    {
        return -65536;
    }

    int tmp=q->q[q->front];

    q->front=(q->front+1)%q->capacity;
    q->size--;

    return tmp;
}
int visitQ(queue* q)
{
    if (q->size==0)
    {
        return -65536;
    }

    return q->q[q->front];
}
void freeQ(queue* q)
{
    free(q->q);
    free(q);
}




