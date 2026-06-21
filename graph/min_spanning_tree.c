#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int from;
    int to;
    int weight;
}Hnode;
typedef struct 
{
    int size;
    int capacity;
    Hnode* heap;
}Heap;

typedef struct Gnode
{
    int Index;
    int Weight;
    struct Gnode* Next;
}Gnode,*PtrToNode;
typedef PtrToNode* Adjlist;

typedef struct 
{
    int Nv;
    int Ne;
    Adjlist G;
    Hnode* Edges;
}Graph;

void swap(Hnode* a,Hnode* b);

Graph* creatG();
void freeG(Graph* Gr);

Heap* creatH(int capacity);
void freeH(Heap* H);
void insert(Heap* H,int from,int to,int weight);
Hnode deletemin(Heap* H);
Hnode visitH(Heap* H);
void percolateup(Heap* H,int p);
void percolatedown(Heap* H,int p);

int find(int* dis,int n);
void setunion(int* dis,int c1,int c2);

Graph* Prim(Graph* G,int sou);
Graph* Kruskal(Graph* G);

int main()
{
    Graph* a=creatG();

    Graph* mina=Kruskal(a);

    freeG(a);
    freeG(mina);
    return 0;
}

void swap(Hnode* a,Hnode* b)
{
    Hnode temp=*a;
    *a=*b;
    *b=temp;
}
Graph* creatG()
{
    Graph* Gr=(Graph*)malloc(sizeof(Graph));
    

    scanf("%d %d",&(Gr->Nv),&(Gr->Ne));
    if (Gr->Ne>(Gr->Nv)*(Gr->Nv-1)/2)
    {
        printf("invalid graph\n");
        free(Gr);
        return NULL;
    }

    Hnode* E=(Hnode*)malloc(sizeof(Hnode)*Gr->Ne);
    int c=0;

    Adjlist adj=(Adjlist)malloc(sizeof(PtrToNode)*(Gr->Nv));

    for (int i=0;i<Gr->Nv;i++)
    {
        adj[i] = (PtrToNode)malloc(sizeof(Gnode));
        adj[i]->Index=i+1;
        adj[i]->Next=NULL;
    }

    int from,to,weight;
    int i=0;
    while (i<Gr->Ne)
    {
        scanf("%d %d %d",&from,&to,&weight);

        if (from==to)
        {
            printf("Input error:Self circle\n");
            continue;
        }

        if (from<1 || from>Gr->Nv || to<1 || to>Gr->Nv)
        {
            printf("Input error:Invalid input\n");
            continue;
        }

        PtrToNode check=adj[from-1]->Next;
        int flag=0;
        while (check!=NULL)
        {
            if (check->Index==to)
            {
                printf("Input error:Edge has already existed\n");
                flag=1;
                break;
            }
            check=check->Next;
        }
        if (flag==1)
            continue;

        E[c].from=from;
        E[c].to=to;
        E[c].weight=weight;
        c++;

        PtrToNode newnode=(PtrToNode)malloc(sizeof(Gnode));
        newnode->Index=to;
        newnode->Weight=weight;

        newnode->Next=adj[from-1]->Next;
        adj[from-1]->Next=newnode;


        PtrToNode newnode2=(PtrToNode)malloc(sizeof(Gnode));
        newnode2->Index=from;
        newnode2->Weight=weight;

        newnode2->Next=adj[to-1]->Next;
        adj[to-1]->Next=newnode2;

        i++;
    }

    Gr->G=adj;
    Gr->Edges=E;

    return Gr;
}
void freeG(Graph* Gr)
{
    if (Gr==NULL) return;

    for (int i=0;i<Gr->Nv;i++)
    {
        PtrToNode p=Gr->G[i];

        while (p)
        {
            PtrToNode tmp=p;
            p=p->Next;
            free(tmp);
        }
    }

    free(Gr->G);

    free(Gr->Edges);

    free(Gr); 
}

Heap* creatH(int capacity)
{
    Hnode* h=(Hnode*)malloc(sizeof(Hnode)*(capacity+1));
    Heap* H=(Heap*)malloc(sizeof(Heap));

    H->heap=h;
    H->capacity=capacity;
    H->size=0;

    return H;
}
void freeH(Heap* H)
{
    free(H->heap);
    free(H);
}
void insert(Heap* H,int from,int to,int weight)
{
    if (H->size+1>H->capacity)
    {
        printf("Heap is full!");
        return;
    }

    H->heap[1+H->size].from=from;
    H->heap[1+H->size].to=to;
    H->heap[1+H->size].weight=weight;
    H->size++;

    percolateup(H,H->size);
}
Hnode deletemin(Heap* H)
{
    if (H->size==0)
    {
        printf("Empty Heap");
        Hnode error={0,0,0};
        return error;
    }

    Hnode result=H->heap[1];

    H->heap[1]=H->heap[H->size];
    H->size--;

    percolatedown(H,1);
    return result;
}
Hnode visitH(Heap* H)
{
    if (H->size)
        return H->heap[1];

    Hnode error={0,0,0};
    return error;
}
void percolateup(Heap* H,int p)
{
    while (p>1)
    {
        int parent=p/2;

        if (H->heap[parent].weight>H->heap[p].weight)
            swap(&(H->heap[parent]),&(H->heap[p]));

        else
            break;

        p=parent;
    }
}
void percolatedown(Heap* H,int p)
{
    while (p*2<=H->size)
    {
        int child=p*2;

        if (child+1<=H->size && H->heap[child].weight > H->heap[child+1].weight)
            child++;

        if (H->heap[p].weight>H->heap[child].weight)
            swap(&(H->heap[child]),&(H->heap[p]));

        else
            break;

        p=child;
    }
}

int find(int* dis,int n)
{
    int root=n;
    while (dis[root]>0)
    {
        root=dis[root];
    }

    int trac=n;
    while (dis[trac]>0)
    {
        int temp=dis[trac];
        dis[trac]=root;
        trac=temp;
    }

    return root;
}
void setunion(int* dis,int c1,int c2)
{
    int r1=find(dis,c1),r2=find(dis,c2);

    if (dis[r1]<dis[r2])
    {
        dis[r1]=dis[r1]+dis[r2];
        dis[r2]=r1;
    }

    else
    {
        dis[r2]=dis[r1]+dis[r2];
        dis[r1]=r2;
    }
}

Graph* Prim(Graph* G,int sou)
{
    if (G==NULL) return NULL;
    Adjlist adj=(Adjlist)malloc(sizeof(PtrToNode)*(G->Nv));

    Hnode* ED=(Hnode*)malloc(sizeof(Hnode)*(G->Nv-1));
    int c=0;

    for (int i=0;i<G->Nv;i++)
    {
        adj[i]=(PtrToNode)malloc(sizeof(Gnode));
        adj[i]->Index=i+1;
        adj[i]->Next=NULL;
    }

    Heap* H=creatH(2 * G->Ne);
    int* hash=(int*)malloc(sizeof(int)*(G->Nv+1));

    for (int i=1;i<=G->Nv;i++)
        hash[i]=0;

    hash[sou]=1;

    PtrToNode check=G->G[sou-1]->Next;
    while (check!=NULL)
    {
        insert(H,G->G[sou-1]->Index,check->Index,check->Weight);
        check=check->Next;
    }

    int min=0;
    int E=0;

    while (H->size!=0 && E<G->Nv-1)
    {
        Hnode tmp=visitH(H);
        if (hash[tmp.to])
        {
            deletemin(H);
            continue;
        }

        hash[tmp.to]=1;
        ED[c].from=tmp.from;
        ED[c].to=tmp.to;
        ED[c].weight=tmp.weight;
        c++;

        min+=tmp.weight;
        E++;

        PtrToNode newnode=(PtrToNode)malloc(sizeof(Gnode));
        newnode->Index=tmp.to;
        newnode->Weight=tmp.weight;
        newnode->Next=adj[tmp.from-1]->Next;
        adj[tmp.from-1]->Next=newnode;

        PtrToNode newnode2=(PtrToNode)malloc(sizeof(Gnode));
        newnode2->Index=tmp.from;
        newnode2->Weight=tmp.weight;
        newnode2->Next=adj[tmp.to-1]->Next;
        adj[tmp.to-1]->Next=newnode2;

        deletemin(H);

        PtrToNode check=G->G[tmp.to-1]->Next;
        while (check!=NULL)
        {
            insert(H,G->G[tmp.to-1]->Index,check->Index,check->Weight);
            check=check->Next;
        }
    }
    Graph* R=(Graph*)malloc(sizeof(Graph));
    R->Nv=G->Nv;
    R->Ne=E;
    R->G=adj;
    R->Edges=ED;

    if (E!=G->Nv-1)
    {
        printf("Min spanning tree doesn't exist\n");
        freeG(R);
        free(hash);
        freeH(H);
        return NULL;
    }

    printf("%d\n",min);
    free(hash);
    freeH(H);
    return R;
}
Graph* Kruskal(Graph* G)
{
    if (G==NULL) return NULL;
    int* dis=(int*)malloc(sizeof(int)*(G->Nv+1));
    for (int i=1;i<=G->Nv;i++)
        dis[i]=-1;

    Heap* H=creatH(G->Ne);
    for (int i=0;i<G->Ne;i++)
        insert(H,G->Edges[i].from,G->Edges[i].to,G->Edges[i].weight);

    Graph* Gr=(Graph*)malloc(sizeof(Graph));
    Adjlist adj=(Adjlist)malloc(sizeof(PtrToNode)*(G->Nv));
    for (int i=0;i<G->Nv;i++)
    {
        adj[i]=(PtrToNode)malloc(sizeof(Gnode));
        adj[i]->Next=NULL;
    }

    Hnode* ED=(Hnode*)malloc(sizeof(Hnode)*(G->Nv-1));

    int k=0;
    int cnt=0;
    while (k<G->Nv-1 && H->size>0)
    {
        Hnode curr=deletemin(H);

        if (find(dis,curr.from)!=find(dis,curr.to))
        {
            setunion(dis,curr.from,curr.to);
            ED[k].from=curr.from;
            ED[k].to=curr.to;
            ED[k].weight=curr.weight;
            k++;

            cnt+=curr.weight;

            PtrToNode newnode=(PtrToNode)malloc(sizeof(Gnode));
            newnode->Index=curr.to;
            newnode->Weight=curr.weight;
            newnode->Next=adj[curr.from-1]->Next;
            adj[curr.from-1]->Next=newnode;

            PtrToNode newnode2=(PtrToNode)malloc(sizeof(Gnode));
            newnode2->Index=curr.from;
            newnode2->Weight=curr.weight;
            newnode2->Next=adj[curr.to-1]->Next;
            adj[curr.to-1]->Next=newnode2;
        }
    }
    Gr->Nv=G->Nv;
    Gr->Ne=k;
    Gr->Edges=ED;
    Gr->G=adj;
    
    if (k!=G->Nv-1)
    {
        freeG(Gr);
        free(dis);
        freeH(H);
        printf("Min spanning tree doesn't exist\n");
        return NULL;
    }

    freeH(H);
    printf("%d\n",cnt);

    return Gr;
}

