#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
typedef struct node
{
    int index;
    int weight;
    struct node* next;
}Gnode;
typedef Gnode* PtrToNode;
typedef PtrToNode* Adjlist;
typedef struct 
{
    Adjlist Adj;
    int Nv;
    int Ne;
}Graph;

typedef struct 
{
    int index;
    int weight;
}heapnode;
typedef struct 
{
    heapnode* heap;
    int size;
}min_heap;

Graph* creat();
void freeG(Graph* G);
int* Dijkstra(Graph* G,int sor);
void swap(heapnode* a,heapnode* b);
void push(min_heap* H,int ind,int wei);
heapnode pop(min_heap* H);
void percolateup(min_heap* H);
void percolatedown(min_heap* H);

int main()
{
    Graph* a=creat();
    int* b=Dijkstra(a,1);
    for (int i=0;i<a->Nv;i++)
    {
        printf("%d ",b[i]);
    }

    free(b);
    freeG(a);
    return 0;
}
Graph* creat()
{
    Graph* Gr=(Graph*)malloc(sizeof(Graph));

    scanf("%d %d",&(Gr->Nv),&(Gr->Ne));

    Adjlist adj=(Adjlist)malloc(sizeof(PtrToNode)*(Gr->Nv));
    if (adj==NULL)
    {
        printf("Error:Memory allocation failed");
        return NULL;
    }

    for (int i=0;i<Gr->Nv;i++)
    {
        adj[i] = (PtrToNode)malloc(sizeof(Gnode));
        if (adj[i]==NULL)
        {
            printf("Error:Memory allocation failed");
            return NULL;
        }
        adj[i]->index=i+1;
        adj[i]->next=NULL;
    }

    //from和to记录输入的边两边的节点索引，i记录已经输入的边数
    int from,to,weight;
    int i=0;
    while (i<Gr->Ne)
    {
        scanf("%d %d %d",&from,&to,&weight);

        //错误输入检查：自我连接
        if (from==to)
        {
            printf("Input error:Self circle\n");
            continue;
        }
        //错误输入检查：节点索引不合法
        if (from<1 || from>Gr->Nv || to<1 || to>Gr->Nv)
        {
            printf("Input error:Invalid input\n");
            continue;
        }

        PtrToNode check=adj[from-1]->next;
        int flag=0;
        while (check!=NULL)
        {
            //错误输入检查：边已经存在
            if (check->index==to)
            {
                printf("Input error:Edge has already existed\n");
                flag=1;
                break;
            }
            check=check->next;
        }
        if (flag==1)
            continue;

        //将边加入邻接表，由于是无向图，所以需要将边的两端都加入邻接表
        PtrToNode newnode=(PtrToNode)malloc(sizeof(Gnode));
        if (newnode==NULL)
        {
            printf("Error:Memory allocation failed");
            return NULL;
        }
        newnode->index=to;
        newnode->weight=weight;

        newnode->next=adj[from-1]->next;
        adj[from-1]->next=newnode;

        PtrToNode newnode2=(PtrToNode)malloc(sizeof(Gnode));
        if (newnode2==NULL)
        {
            printf("Error:Memory allocation failed");
            return NULL;
        }
        newnode2->index=from;
        newnode2->weight=weight;

        newnode2->next=adj[to-1]->next;
        adj[to-1]->next=newnode2;

        i++;
    }

    Gr->Adj=adj;

    return Gr;
}
int* Dijkstra(Graph* G,int sor)
{
    if (G == NULL || sor < 1 || sor > G->Nv)
        return NULL;

    int* dist=(int*)malloc(sizeof(int)*G->Nv);
    int* visited=(int*)malloc(sizeof(int)*G->Nv);
    min_heap* H=(min_heap*)malloc(sizeof(min_heap));

    int heapCapacity = G->Ne * 2 + 2;
    if (heapCapacity < G->Nv + 1)
        heapCapacity = G->Nv + 1;

    heapnode* h=(heapnode*)malloc(sizeof(heapnode)*heapCapacity);
    H->heap=h;
    H->size=0;
    int* result=(int*)malloc(sizeof(int)*G->Nv);

    for (int i=0;i<G->Nv;i++)
    {
        dist[i]=INT_MAX;
        visited[i]=0;
        result[i]=INT_MAX;
    }

    dist[sor-1]=0;
    result[sor-1]=0;
    push(H,sor,0);

    while (H->size > 0)
    {
        heapnode out=pop(H);
        if (visited[out.index-1])
            continue;
            

        result[out.index-1]=out.weight;
        visited[out.index-1]=1;

        PtrToNode check=G->Adj[out.index-1]->next;
        while (check!=NULL)
        {
            if (!visited[check->index-1] && check->weight+out.weight<dist[check->index-1])
            {
                dist[check->index-1]=check->weight+out.weight;
                push(H,check->index,dist[check->index-1]);
            }
            check=check->next;
        }
    }
    free(dist);
    free(visited);
    free(h);
    free(H);
    return result;
}
void swap(heapnode* a,heapnode* b)
{
    heapnode tmp=*a;
    *a=*b;
    *b=tmp;
}
void push(min_heap* H,int ind,int wei)
{
    H->size++;
    (H->heap)[H->size].index=ind;
    (H->heap)[H->size].weight=wei;
    percolateup(H);
}
heapnode pop(min_heap* H)
{
    heapnode out=H->heap[1];
    H->heap[1]=H->heap[H->size];
    H->size--;
    percolatedown(H);
    return out;
}
void percolateup(min_heap* H)
{
    int p=H->size;
    while (p>1)
    {
        int parent=p/2;
        if (H->heap[p].weight<H->heap[parent].weight)
            swap(&(H->heap[p]),&(H->heap[parent]));
        else
            break;

        p=parent;
    }
}
void percolatedown(min_heap* H) 
{
    int p=1;
    while (p*2<=H->size)
    {
        int child=p*2;
        while (child+1<=H->size && H->heap[child+1].weight<H->heap[child].weight)
            child++;

        if (H->heap[p].weight>H->heap[child].weight)
            swap(&(H->heap[p]),&(H->heap[child]));
        else
            break;

        p=child;
    }
}
void freeG(Graph* G)
{
    for (int i=0;i<G->Nv;i++)
    {
        PtrToNode p=G->Adj[i];
        while (p!=NULL)
        {
            PtrToNode temp=p;
            p=p->next;
            free(temp);
        }
    }
    free(G->Adj);

    free(G);
}