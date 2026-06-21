#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_NAME 26*26*26
#define MAX_VERTEX 1000

int hash[MAX_NAME];
int matrix[MAX_VERTEX][MAX_VERTEX]={0};
int Nv=0;

int ID(char* name)
{
    return (name[0]-'A')*26*26+(name[1]-'A')*26+(name[2]-'A');
}
int find(char* name)
{
    int id=ID(name);

    if (hash[id]==-1)
    {
        hash[id]=Nv++;
    }

    return hash[id];
}
//广度优先遍历
int BFS(int s,int t,int n,int parent[])
{
    int visited[MAX_VERTEX]={0};
    int queue[MAX_VERTEX],front=0,rear=0;
    visited[s]=1;
    queue[rear++] = s;
    parent[s]=-1;

    while (front<rear)
    {
        int u=queue[front++];

        for (int v=0;v<n;v++)
        {
            if (!visited[v] && matrix[u][v]>0)
            {
                visited[v]=1;
                parent[v]=u;
                if (v==t) return 1;
                queue[rear++] = v;
            }
        }
    }
    return 0;
}
//Edmonds-Karp算法
int maxflow(int s,int t,int n)
{
    int flow=0;
    int p[MAX_VERTEX];

    while (BFS(s,t,n,p))
    {
        int min=INT_MAX;

        for (int v=t;v!=s;v=p[v])
        {
            int u=p[v];
            if (matrix[u][v]<min)
                min=matrix[u][v];
        }

        for (int v=t;v!=s;v=p[v])
        {
            int u=p[v];
            matrix[u][v]-=min;
            matrix[v][u]+=min;
        }
        flow+=min;
    }
    return flow;
}
int main()
{
    char sou[4],dest[4];
    int N;
    scanf(" %s %s %d",sou,dest,&N);

    for (int i=0;i<MAX_NAME;i++)
        hash[i]=-1;

    int si=find(sou);
    int di=find(dest);

    for (int i=0;i<N;i++)
    {
        char from[4],to[4];
        int w;
        scanf(" %s %s %d",from,to,&w);

        int fi=find(from);
        int ti=find(to);

        matrix[fi][ti]+=w;
    }
    printf("%d",maxflow(si,di,Nv));
    return 0;
}