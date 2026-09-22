#include <stdio.h>
#include <stdlib.h>
#include <AVL.h>

void update(PtrToNode AVL)
{
    if (AVL==NULL)
        AVL->height=-1;
    else
    {
        int h_l = AVL->left==NULL?-1:AVL->left->height;
        int h_r = AVL->right==NULL?-1:AVL->right->height;

        AVL->height = h_l>h_r?h_l+1:h_r+1;
        AVL->bf = h_l-h_r;
    }

}

PtrToNode RightRot(PtrToNode AVL)
{
    PtrToNode L = AVL->left;

    AVL->left = L->right;
    L->right = AVL;

    update(AVL);
    update(L);

    return L;
}

PtrToNode LeftRot(PtrToNode AVL)
{
    PtrToNode R = AVL->right;

    AVL->right = R->left;
    R->left = AVL;

    update(AVL);
    update(R);

    return R;
}

PtrToNode insert(PtrToNode AVL,int n)
{
    if (AVL==NULL)
    {
        PtrToNode newnode = (PtrToNode)malloc(sizeof(node));
        newnode->data = n;
        newnode->bf = 0;
        newnode->height = 0;
        newnode->left = NULL;
        newnode->right = NULL;

        return newnode;
    }
        
    else if (n < AVL->data)
        AVL->left=insert(AVL->left,n);

    else if (n > AVL->data)
        AVL->right = insert(AVL->right,n);


    update(AVL);

    if (AVL->bf > 1 && n < AVL->left->data)
        return RightRot(AVL);

    if (AVL->bf < -1 && AVL->right->data < n)
        return LeftRot(AVL);

    if (AVL->bf > 1 && n > AVL->left->data)
    {
        AVL->left = LeftRot(AVL->left);
        return RightRot(AVL);
    }

    if (AVL->bf < -1 && n < AVL->right->data)
    {
        AVL->right = RightRot(AVL->right);
        return LeftRot(AVL);
    }

    return AVL;
}