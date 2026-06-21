#include <stdio.h>
#include <stdlib.h>

typedef int* heap;

void swap(int* a,int* b);
void percolatedown(heap h,int n,int i);
void percolateup(heap h,int n,int i);
heap build_heap(int* list,int n);
int delete(heap h,int *n);
void insert(heap h,int *n,int x);
void heapsort(int * list,int n);

int main()
{
    int a[6]={543,5432,654,12,754,234};
    
    int n=6;
    heap H=build_heap(a,n);
    for (int i=1;i<=6;i++)
        printf("%d ",H[i]);
    printf("\n");

    delete(H,&n);
    for (int i=1;i<=n;i++)
        printf("%d ",H[i]);
    printf("\n");

    insert(H,&n,312);
    for (int i=1;i<=n;i++)
        printf("%d ",H[i]);
    printf("\n");

    heapsort(a,6);
    for (int i=0;i<6;i++)
        printf("%d ",a[i]);

    free(H);
    return 0;
}
void swap(int* a,int* b)
{
    int temp=*a;
    *a=*b;
    *b=temp;
}
void heapsort(int * list,int n)
{
    if (n <= 0) return;
    heap H = build_heap(list, n);
    int m = n;
    for (int i = 0; i < n; i++) 
    {
        int v = delete(H, &m);
        list[i] = v;
    }
    free(H);
}
void percolatedown(heap h,int n,int i)
{
    int pos=i;
    int child;
    while (pos*2<=n)
    {
        child=pos*2;
        if (child+1<=n && h[child+1]<h[child]) child++;

        if (h[pos]<=h[child]) break;

        swap(&h[pos],&h[child]);
        pos=child;
    }
}
void percolateup(heap h,int n,int i)
{
    int pos=i;
    int parent;
    while (pos>1)
    {
        parent=pos/2;
        
        if (h[pos]>=h[parent]) break;

        swap(&h[pos],&h[parent]);
        pos=parent;
    }
}
heap build_heap(int* list,int n)
{
    heap H=(heap)malloc(sizeof(int)*(n+1));
    for (int i=1;i<=n;i++)
        H[i]=list[i-1];
    for (int i=n/2;i>=1;i--)
        percolatedown(H,n,i);
    return H;
}
int delete(heap h,int *n)
{
    if (*n<=0) return -32768;
    int tmp=h[1];
    h[1]=h[*n];
    (*n)--;
    percolatedown(h,*n,1);
    return tmp;
}
void insert(heap h,int *n,int x)
{
    (*n)++;
    h[*n]=x;
    percolateup(h,*n,*n);
}