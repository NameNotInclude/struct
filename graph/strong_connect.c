#include <stdlib.h>
#include <stdio.h>
typedef struct Gnode 
{
    int Index;
    struct Gnode* Next;
}Gnode;

typedef Gnode* PtrToNode;
typedef PtrToNode* Adjlist;

typedef struct 
{
    int Nv;
    int Ne;
    Adjlist G;
}Graph;

//全局变量，用于tarjan
static int *idx, *low, *onStack, *stack;
static int counter, top;
//idx:第几个被发现
//low:向上能回溯最早的节点

Graph* creat();
void freeG(Graph* Gr);
void strong_connect(Graph* Gr);
void strong_connect_inner(Graph* Gr,int v);
int main()
{
    Graph* a=creat();
    strong_connect(a);
    freeG(a);
    return 0;
}

Graph* creat()
{
    Graph* Gr=(Graph*)malloc(sizeof(Graph));

    scanf("%d %d",&(Gr->Nv),&(Gr->Ne));

    Adjlist adj=(Adjlist)malloc(sizeof(PtrToNode)*(Gr->Nv));

    for (int i=0;i<Gr->Nv;i++)
    {
        adj[i] = (PtrToNode)malloc(sizeof(Gnode));
        adj[i]->Index=i+1;
        adj[i]->Next=NULL;
    }

    int from,to;
    int i=0;
    while (i<Gr->Ne)
    {
        scanf("%d %d",&from,&to);

        if (from==to)
        {
            printf("Input error:Self circle");
            continue;
        }

        if (from<1 || from>Gr->Nv || to<1 || to>Gr->Nv)
        {
            printf("Input error:Invalid input");
            continue;
        }

        PtrToNode check=adj[from-1]->Next;
        int flag=0;
        while (check!=NULL)
        {
            if (check->Index==to)
            {
                printf("Input error:Edge has already existed");
                flag=1;
                break;
            }
            check=check->Next;
        }
        if (flag==1)
            continue;

        PtrToNode newnode=(PtrToNode)malloc(sizeof(Gnode));
        newnode->Index=to;

        newnode->Next=adj[from-1]->Next;
        adj[from-1]->Next=newnode;

        i++;
    }

    Gr->G=adj;

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

    free(Gr);
}

void strong_connect(Graph* Gr)
{
    int n=Gr->Nv;
    idx=(int*)malloc(sizeof(int)*(n+1));
    low=(int*)malloc(sizeof(int)*(n+1));
    onStack=(int*)malloc(sizeof(int)*(n+1));
    stack=(int*)malloc(sizeof(int)*(n+1));
    counter=0;
    top=0;

    for(int i=1;i<=n;i++)
    {
        idx[i]=-1;
        onStack[i]=0;
    }

    for (int i=1;i<=n;i++)
        if (idx[i]==-1)
            strong_connect_inner(Gr,i);

    free(idx);
    free(low);
    free(onStack);
    free(stack);
}
void strong_connect_inner(Graph* Gr,int v)
{
    idx[v]=low[v]=counter++;
    stack[top++]=v;
    onStack[v]=1;

    PtrToNode p=Gr->G[v-1]->Next;
    while (p)
    {
        int w=p->Index;

        //w没访问，树枝边
        if(idx[w]==-1)
        {
            strong_connect_inner(Gr,w);

            //子节点如果能去更早的地方，将low值传回自己
            if (low[w]<low[v])
                low[v]=low[w];
        }

        //在栈中，是返祖边
        else if (onStack[w])
        {
            //用祖先的idx更新自己的low值
            if (idx[w]<low[v])
                low[v]=idx[w];
        }
        p=p->Next;
    }

    //强连通
    if (low[v]==idx[v])
    {
        int w;
        //从栈中弹出，直至弹出自己
        do
        {
            w=stack[--top];
            onStack[w]=0;
            printf("%d ",w);
        } while (w!=v);
        printf("\n");
    }
}
