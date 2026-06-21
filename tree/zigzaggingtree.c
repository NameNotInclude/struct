#include <stdio.h>
#include <stdlib.h>
typedef struct Tree
{
    int data;
    struct Tree* left;
    struct Tree* right;
}tree;
typedef tree* ptr;

int value[30][30];
int top[30];
int depth=0;

int find(int* from,int n,int tar)
{
    for (int i=0;i<n;i++)
        if (from[i]==tar)
            return i;
    return -1;
}
ptr creat(int* ino,int* posto,int instart,int inend,int poststart,int postend)
{
    if (instart>inend ) return NULL;
    
    ptr result=(ptr)malloc(sizeof(tree));
    result->data=posto[postend];
    
    int pos=find(ino+instart,inend-instart+1,posto[postend]);
    result->left=creat(ino,posto,instart,instart+pos-1,poststart,poststart+pos-1);
    result->right=creat(ino,posto,instart+pos+1,inend,poststart+pos,postend-1);
    return result;
}
void in_traversal(ptr t,int n)
{
    if (t==NULL) return;
    value[n][top[n]++]=t->data;
    if (n+1>=depth) depth=n+1;
    in_traversal(t->left,n+1);
    in_traversal(t->right,n+1);
}
int main()
{
    int n;
    scanf("%d",&n);
    int* post=(int*)malloc(sizeof(int)*n);
    int* in=(int*)malloc(sizeof(int)*n);
    for (int i=0;i<n;i++)
        scanf("%d",in+i);
    for (int i=0;i<n;i++)
        scanf("%d",post+i);
    
    ptr T=creat(in,post,0,n-1,0,n-1);

    in_traversal(T,0);
    printf("%d",value[0][0]);
    for (int i=1;i<depth;i++)
    {
        if (i%2==0)
            for (int j=top[i]-1;j>=0;j--)
                printf(" %d",value[i][j]);
        else
            for (int j=0;j<top[i];j++)
                printf(" %d",value[i][j]);
    }
    free(post);
    free(in);
    return 0;
}
/*
8
12 11 20 17 1 15 8 5
12 20 17 11 15 8 5 1

1 11 5 8 17 12 20 15
*/

