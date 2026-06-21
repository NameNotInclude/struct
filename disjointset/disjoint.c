#include <stdio.h>
#include <stdlib.h>
typedef int* diset;
diset creat(int n);
void unionset_size(diset D,int c1,int c2);
int find(diset D,int c);
int check(diset D,int c1,int c2);
int main()
{
    int n;
    scanf("%d",&n);
    diset D=creat(n);

    unionset_size(D, 1, 2);
    unionset_size(D, 3, 4);
    unionset_size(D, 1, 3);


    printf("1 and 4 connected? %d\n", check(D, 1, 4));
    printf("1 and 5 connected? %d\n", check(D, 1, 5));
    
    free(D);
    return 0;
}
diset creat(int n)
{
    diset D=(diset)malloc(sizeof(int)*(n+1));
    for (int i=1;i<=n;i++)
    {
        D[i]=-1;
    }
    return D; 
}
void unionset_size(diset D,int c1,int c2)
{
    int x=find(D,c1);

    int y=find(D,c2);

    if (D[x]<D[y])
    {
        int num=D[x]+D[y];
        D[y]=x;
        D[x]=num;
    }
    else
    {
        int num=D[x]+D[y];
        D[x]=y;
        D[y]=num;
    }
}
int find(diset D,int c)
{
    if (D[c]<0) return c;
    return D[c]=find(D,D[c]);//path compression
}
int check(diset D,int c1,int c2)
{
    int a=find(D,c1);
    int b=find(D,c2);
    return (a==b)?1:0;
}