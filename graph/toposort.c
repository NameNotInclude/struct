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


Graph* creat();
void freeG(Graph* Gr);
void toposort(Graph* Gr);

int main()
{
    Graph* a=creat();
    toposort(a);
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

void toposort(Graph* Gr)
{
    int* indegree=(int*)malloc(sizeof(int)*Gr->Nv);
    for (int i=0;i<Gr->Nv;i++)
        indegree[i]=0;

    for (int i=0;i<Gr->Nv;i++)
    {
        PtrToNode check=Gr->G[i]->Next;

        while (check)
        {
            indegree[check->Index-1]++;
            check=check->Next;
        }
    }

    int* stack=(int*)malloc(sizeof(int)*Gr->Nv);
    int top=0;
    for (int i=0;i<Gr->Nv;i++)
        if (indegree[i]==0)
            stack[top++]=i+1;
    int out=0;

    while (out<Gr->Nv)
    {
        int output;
        if (top == 0) 
        {
            printf("Error: Graph has a cycle!\n");
            break;
        }
        
        if (top!=0)
        {
            output=stack[top-1];
            printf("%d ",stack[--top]);
        }
        out++;
    
        PtrToNode check=Gr->G[output-1]->Next;

        while (check)
        {
            indegree[check->Index-1]--;
            if (indegree[check->Index-1]==0)
                stack[top++]=check->Index;
            check=check->Next;
        }
    }

    free(indegree);
    free(stack);
}

