/*
This program supports common operands on complete BST
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define min(a,b) ((a)<(b)?(a):(b))
typedef struct Tree
{
    int data;
    struct Tree* left;
    struct Tree* right;
    int lflag;  // 1 if left is a thread to predecessor, 0 if child
    int rflag; // 1 if right is a thread to successor, 0 if child
}tree;
typedef tree* root;

tree* build_tree(int* a,int n);
tree* build_tree_inner(int* a, int start,int end);
int position(int* a,int start,int end);
void CreateInThread(tree *T);
void Inthread(tree* T,tree** pre);

tree* find(tree* T,int tar);
tree* findmin(tree* T);
tree* findmax(tree* T);

void preorder_traversal_recursive(root T);
void inorder_traversal_recursive(root T);
void postorder_traversal_recursive(root T);

void postorder_traversal_iterative(root T);
void inorder_traversal_iterative(root T);
void preorder_traversal_iterative(root T);

void preorder_traversal_thread(root T);
void inorder_traversal_thread(root T);


tree* insert(tree* T,int val);
tree* delete(tree* T,int val);

void quick_sort(int arr[],int start,int end);
int pivot(int arr[],int start,int end);

void destroy(root T);
void destroy_thread(tree* T);

int main()
{
    int a[10]={10,9,8,7,6,5,4,3,2,1};
    tree* T=build_tree(a,10);
    //CreateInThread(T);
    inorder_traversal_iterative(T);
    
    destroy_thread(T);
    return 0;
}



//creat tree
tree* build_tree(int* a,int n)
{
    quick_sort(a,0,n-1);
    return build_tree_inner(a,0,n-1);
}
tree* build_tree_inner(int* a, int start,int end)
{
    if (start>end) return NULL;
    int p=position(a,start,end);
    tree* root=(tree*)malloc(sizeof(tree));
    root->data=a[p];
    root->lflag=0;
    root->rflag=0;
    root->left=build_tree_inner(a,start,p-1);
    root->right=build_tree_inner(a,p+1,end);

    return root;
}
int position(int* a,int start,int end)
{
    int n=end-start+1;
    int depth=log2(n+1);
    int last=n-pow(2,depth)+1;
    int left_n=pow(2,depth-1)-1+min(last,pow(2,depth-1));
    return start+left_n;
}

//depth
int depth(root T)
{
    if (T==NULL) return 0;
    int left_depth = (T->lflag == 0) ? depth(T->left) : 0;
    int right_depth = (T->rflag == 0) ? depth(T->right) : 0;
    return (left_depth > right_depth ? left_depth : right_depth) + 1;
}

//add thread
void CreateInThread(tree *T)
{
    tree *pre = NULL;
    Inthread(T, &pre);
    if (pre!=NULL)
    {
        pre->right=NULL;
        pre->rflag=1;
    }
}
void Inthread(tree* T,tree** pre)
{
    if (T==NULL) return ;

    if (T->lflag == 0)
        Inthread(T->left,pre);

    if (T->left==NULL)
    {
        T->left=*pre;
        T->lflag=1;
    }
    if (*pre!=NULL && (*pre)->right==NULL)
    {
        (*pre)->right=T;
        (*pre)->rflag=1;
    }
    *pre=T;
    if (T->rflag == 0)
        Inthread(T->right,pre);
}

void CreatePreThread(tree *T)
{
    tree *pre = NULL;
    Prethread(T, &pre);
    if (pre != NULL) 
    {
        pre->right = NULL;
        pre->rflag = 1;
    }
}
void Prethread(tree* T,tree** pre)
{
    if (T==NULL) return ;

    if (T->left==NULL)
    {
        T->left=*pre;
        T->lflag=1;
    }
    if (*pre!=NULL && (*pre)->right==NULL)
    {
        (*pre)->right=T;
        (*pre)->rflag=1;
    }
    *pre=T;
    if (T->lflag == 0)
        Prethread(T->left,pre);
    if (T->rflag == 0)
        Prethread(T->right,pre);
}

void CreatePostThread(tree *T)
{
    tree *pre = NULL;
    Postthread(T, &pre);
    if (pre != NULL) 
    {
        pre->right = NULL;
        pre->rflag = 1;
    }
}
void Postthread(tree* T,tree** pre)
{
    if (T==NULL) return ;
    
    if (T->lflag == 0)
        Postthread(T->left,pre);

    if (T->rflag == 0)
        Postthread(T->right,pre);

    if (T->left==NULL)
    {
        T->left=*pre;
        T->lflag=1;
    }
    if (*pre!=NULL && (*pre)->right==NULL)
    {
        (*pre)->right=T;
        (*pre)->rflag=1;
    }
    *pre=T;
}

//find functions
tree* find(tree* T,int tar)
{
   while(T)
   {
        if (tar==T->data)
            return T;
        if (tar<T->data)
            T=T->left;
        else
            T=T->right;
   }
   return NULL;
}
tree* findmin(tree* T)
{
    tree* t1=T;
    if (t1==NULL) 
        return NULL;
    while (t1->lflag==0 && t1->left!=NULL)
        t1=t1->left;
    return t1;
}
tree* findmax(tree* T)
{
    tree* t1=T;
    if (t1==NULL)
        return NULL;
    while (t1->rflag==0 && t1->right!=NULL)
        t1=t1->right;
    return t1;
}

//recursive traversal
void preorder_traversal_recursive(root T)
{
    if (T==NULL)
        return ;
    printf("%d ",T->data);
    if (T->lflag==0)
        preorder_traversal_recursive(T->left);
    if (T->rflag==0)
        preorder_traversal_recursive(T->right);
}
void inorder_traversal_recursive(root T)
{
    if (T==NULL)
        return ;
    if (T->lflag==0)
        inorder_traversal_recursive(T->left);
    printf("%d ",T->data);
    if (T->rflag==0)
        inorder_traversal_recursive(T->right);
}
void postorder_traversal_recursive(root T)
{
    if (T==NULL)
        return ;
    if (T->lflag==0)
        postorder_traversal_recursive(T->left);
    if (T->rflag==0)
        postorder_traversal_recursive(T->right);
    printf("%d ",T->data);
}

//iterative traversal
void preorder_traversal_iterative(root T)
{
    if (T==NULL) return ;

    int d=depth(T);
    tree** stack=(tree**)malloc(sizeof(tree*)*d);
    int top=0;

    stack[top++]=T;

    while (top>0)
    {
        tree* curr=stack[--top];

        printf("%d ",curr->data);

        if (curr->rflag==0 && curr->right != NULL)
            stack[top++]=curr->right;
        if (curr->lflag==0 && curr->left != NULL)
            stack[top++]=curr->left;
    }
    free(stack);
}
void inorder_traversal_iterative(root T)
{
    if (T==NULL) return ;

    int d=depth(T);
    tree** stack=(tree**)malloc(sizeof(tree*)*d);
    int top=0;
    tree* check=T;
   while (check!=NULL || top>0)
   {
        while (check != NULL && check->lflag == 0)
        {
            stack[top++]=check;
            check=check->left;
        }

        //遇到线索，手动加入栈中
        if (check != NULL)
           stack[top++] = check;

        check=stack[--top];
        printf("%d ",check->data);

        check=check->rflag==0?check->right:NULL;
   } 
   
   free(stack);
}
void postorder_traversal_iterative(root T)
{
    if (T==NULL) return;

    int d=depth(T);

    tree** stack=(tree**)malloc(sizeof(tree*)*d);
    int top=0;

    tree* last=NULL;
    tree* curr=T;

    while (curr!=NULL || top>0)
    {
        while (curr !=NULL && curr->lflag==0)
        {
            stack[top++]=curr;
            curr=curr->left;
        }

        if (curr != NULL)
            stack[top++] = curr;

        curr=stack[top-1];

        if (curr->rflag==0 && curr->right!=NULL && curr->right!=last)
            curr=curr->right;
        else
        {
            top--;
            printf("%d ",curr->data);
            last=curr;
            curr=NULL;//下一轮的curr从栈顶取
        }
    }
    free(stack);
}

//thread tree traversal
void preorder_traversal_thread(root T)
{
    tree* check=T;
    if (check==NULL) return ;

    while (check!=NULL)
    {
        printf("%d ",check->data);

        if (check->lflag==0)
            check=check->left;
        else
            check=check->right;
    }
}
void inorder_traversal_thread(root T)
{
    tree* check=T;
    if (check==NULL) return ;

    while (check->lflag==0)
        check=check->left;

    while (check!=NULL)
    {
        printf("%d ",check->data);

        if (check->rflag==1)
            check=check->right;

        else
        {
            check=check->right;
            while (check!=NULL && check->lflag==0)//segmentation fault
                check=check->left;
        }
    }
}
void levelorder(tree* T)
{
    if (T==NULL) return;

    tree* queue[1001]={0};
    int front=0,rear=0;

    queue[rear]=T;
    rear=(rear+1)%1001;

    while (front!=rear)
    {
        tree* curr=queue[front];
        front=(front+1)%1001;
        printf("%d ",curr->data);

        if (curr->left!=NULL)
        {
            queue[rear]=curr->left;
            rear=(rear+1)%1001;
        }
        if (curr->right!=NULL)
        {
            queue[rear]=curr->right;
            rear=(rear+1)%1001;
        }
    }
}

//insert and delete
tree* insert(tree* T,int val)
{
    if (T==NULL)
    {
        T=(tree*)malloc(sizeof(tree));
        T->data=val;
        T->left=NULL;
        T->right=NULL;
    }
    else
    {
        if (val < T->data)
            T->left=insert(T->left,val);
        else if (val > T->data)
            T->right=insert(T->right,val);
        // If val == T->data, do nothing (no duplicates)
    }
    return T;
}
tree* delete(tree* T ,int val)
{
    if (T==NULL)
        return NULL;
    else if (val < T->data)
        T->left=delete(T->left,val);
    else if (val > T->data)
        T->right=delete(T->right,val);
    else
    {
        if( T->left!= NULL && T->lflag==0 && T->right!=NULL && T->rflag==0)
        {
            tree* temp=findmin(T->right);
            T->data=temp->data;
            T->right=delete(T->right,temp->data);
        }
        else
        {
            tree* temp=T;
            if (T->left==NULL && T->lflag==0)
                T=T->right;
            else if (T->right==NULL && T->lflag==0)
                T=T->left;
            free(temp);
        }
    }
    return T;
}

//sort
void quick_sort(int arr[], int start, int end)
{
    int p;
    if (start < end)
    {
        p = pivot(arr, start, end);
        quick_sort(arr, start, p - 1);
        quick_sort(arr, p + 1, end);
    }
}
int pivot(int arr[], int start, int end)
{
    int mid = (start + end) / 2;

    if (arr[end] < arr[start]) 
    {
        int temp = arr[end]; arr[end] = arr[start]; arr[start] = temp;
    }
    if (arr[end] < arr[mid]) 
    {
        int temp = arr[end]; arr[end] = arr[mid]; arr[mid] = temp;
    }
    if (arr[mid] > arr[start]) 
    {
        int temp = arr[mid]; arr[mid] = arr[start]; arr[start] = temp;
    }

    int p = arr[start];
    int low = start;
    int high = end;

  
    while (low < high) 
    {
        while (low < high && arr[high] >= p) high--;
        while (low < high && arr[low] <= p) low++;

        int temp = arr[low];
        arr[low] = arr[high];
        arr[high] = temp;
    }

    arr[start] = arr[low];
    arr[low] = p;

    return low;
}


void destroy(root T) 
{
    if (T == NULL) return;
    destroy(T->left);
    destroy(T->right);
    free(T);
}
void destroy_thread(tree* T)
{
    if (T==NULL) return;
    if (T->lflag==0) destroy_thread(T->left);
    if (T->rflag==0) destroy_thread(T->right);
    free(T);
}
