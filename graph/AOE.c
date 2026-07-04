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
    Adjlist Re_Adj;
    int Nv;
    int Ne;
}Graph;

Graph* creat();
void freeG(Graph* G);
void critical(Graph* G,int sor,int des);

int main()
{
    Graph* a=creat();
    critical(a,1,6);
    freeG(a);
    return 0;
}

Graph* creat()
{
    Graph* Gr=(Graph*)malloc(sizeof(Graph));

    scanf("%d %d",&(Gr->Nv),&(Gr->Ne));

    Adjlist adj=(Adjlist)malloc(sizeof(PtrToNode)*(Gr->Nv));
    Adjlist re_adj=(Adjlist)malloc(sizeof(PtrToNode)*(Gr->Nv));
    if (adj==NULL || re_adj==NULL)
    {
        printf("Error:Memory allocation failed");
        return NULL;
    }

    for (int i=0;i<Gr->Nv;i++)
    {
        adj[i] = (PtrToNode)malloc(sizeof(Gnode));
        re_adj[i]=(PtrToNode)malloc(sizeof(Gnode));
        if (adj[i]==NULL || re_adj[i]==NULL)
        {
            printf("Error:Memory allocation failed");
            return NULL;
        }
        adj[i]->index=i+1;
        adj[i]->next=NULL;

        re_adj[i]->index=i+1;
        re_adj[i]->next=NULL;
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

        //将边加入邻接表
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

        newnode2->next=re_adj[to-1]->next;
        re_adj[to-1]->next=newnode2;

        i++;
    }

    Gr->Adj=adj;
    Gr->Re_Adj=re_adj;

    return Gr;
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

    for (int i=0;i<G->Nv;i++)
    {
        PtrToNode p=G->Re_Adj[i];
        while (p!=NULL)
        {
            PtrToNode temp=p;
            p=p->next;
            free(temp);
        }
    }
    free(G->Re_Adj);


    free(G);
}
void critical(Graph* G,int sor,int des)
{
    if (G==NULL || sor<1 || sor>G->Nv || des<1 || des>G->Nv)
        return;

    int* EC=(int*)malloc(sizeof(int)*(G->Nv+1));
    int* LC=(int*)malloc(sizeof(int)*(G->Nv+1));
    int* in=(int*)malloc(sizeof(int)*(G->Nv+1));
    int* topo=(int*)malloc(sizeof(int)*(G->Nv+1));
    int* stack=(int*)malloc(sizeof(int)*(G->Nv+1));

    if (EC==NULL || LC==NULL || in==NULL || topo==NULL || stack==NULL)
    {
        printf("Error:Memory allocation failed\n");
        free(EC);
        free(LC);
        free(in);
        free(topo);
        free(stack);
        return;
    }

    for (int i=1;i<=G->Nv;i++)
    {
        in[i]=0;
        EC[i]=INT_MIN;
        LC[i]=INT_MAX;
    }
    EC[sor]=0;

    for (int i=0;i<G->Nv;i++)
    {
        PtrToNode check=G->Adj[i]->next;
        while (check)
        {
            in[check->index]++;
            check=check->next;
        }
    }

    int top=0;
    int count=0;
    for (int i=1;i<=G->Nv;i++)
        if (in[i]==0)
            stack[top++]=i;

    while (top>0)
    {
        int out=stack[--top];
        topo[count++]=out;

        PtrToNode check=G->Adj[out-1]->next;
        while (check)
        {
            if (EC[out]!=INT_MIN && EC[out]+check->weight>EC[check->index])
                EC[check->index]=EC[out]+check->weight;

            in[check->index]--;
            if (in[check->index]==0)
                stack[top++]=check->index;

            check=check->next;
        }
    }

    if (count!=G->Nv)
    {
        printf("Error: Graph has a cycle!\n");
        free(EC);
        free(LC);
        free(in);
        free(topo);
        free(stack);
        return;
    }

    if (EC[des]==INT_MIN)
    {
        printf("Error: No path from %d to %d!\n",sor,des);
        free(EC);
        free(LC);
        free(in);
        free(topo);
        free(stack);
        return;
    }

    LC[des]=EC[des];

    for (int i=count-1;i>=0;i--)
    {
        int v=topo[i];
        PtrToNode check=G->Adj[v-1]->next;

        while (check)
        {
            if (LC[check->index]!=INT_MAX && LC[check->index]-check->weight<LC[v])
                LC[v]=LC[check->index]-check->weight;
            check=check->next;
        }
    }

    printf("Critical activities:\n");

    for (int i=1;i<=G->Nv;i++)
    {
        PtrToNode check=G->Adj[i-1]->next;
        while (check)
        {
            if (EC[i]!=INT_MIN && LC[check->index]!=INT_MAX)
            {
                int e=EC[i];
                int l=LC[check->index]-check->weight;
                if (e==l)
                    printf("%d -> %d (weight=%d)\n",i,check->index,check->weight);
            }
            check=check->next;
        }
    }
    
    free(in);
    free(stack);
    free(EC);
    free(LC);
    free(topo);
}

