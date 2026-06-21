#include <stdio.h>
#include <stdlib.h>
#include "linkedlist.h"
struct _node 
{
    int value;
    struct _node *next;
};
List list_create()
{
    List h;
    h.head=NULL;
    h.tail=NULL;
    return h;
}
void list_free(List *list)
{
    Node* prev=list->head;
    Node* curr;
    while (curr!=list->tail)
    {
        curr=prev->next;
        free(prev);
        prev=curr;
    }
    list->head=NULL;
    list->tail=NULL;
}
void list_append(List *list, int v)
{
    Node* newnode=(Node *)malloc(sizeof(Node));
    newnode->value=v;
    newnode->next=NULL;
    if (list->tail==NULL)
    {
        list->head=newnode;
        list->tail=newnode;
    }
    else
    {
        list->tail->next=newnode;
        list->tail=newnode;
    }
}
void list_insert(List *list, int v)
{
    Node* newnode=(Node*)malloc(sizeof(Node));
    newnode->value=v;
    newnode->next=list->head;
    if (list->tail==NULL)
    {
        list->tail=newnode;
    }
    list->head=newnode;
}
void list_set(List *list, int index, int v)
{
    int i=0;
    Node* check=list->head;
    while (i<index && check!=NULL)
    {
        check=check->next;
        i++;
    }
    if (check==NULL) return;
    check->value=v;
}
int list_get(List *list, int index)
{
    int i=0;
    Node *check=list->head;
    while (i<index)
    {
        i++;
        check=check->next;
    }
    return check->value;
}
int list_size(List *list)
{
    int i=0;
    Node *check=list->head;
    while (check!=NULL)
    {
        i++;
        check=check->next;
    }
    return i;
}
int list_find(List *list, int v)
{
    int i=0;
    Node* check=list->head;
    while (check!=NULL && check->value!=v)
    {
        check=check->next;
        i++;
    }
    if (check==NULL) return -1;
    return i;
}
void list_remove(List *list, int v)
{
    if (list->head->value==v)
    {
        Node* temp=list->head;
        list->head=list->head->next;
        free(temp);
    }
    else
    {
        Node* prev=list->head;
        Node* curr=prev->next;
        while (curr->value!=v)
        {
            prev=curr;
            curr=curr->next;
        }
        prev->next=curr->next;
        if (curr==list->tail)
        {
            list->tail=prev;
        }
        free(curr);
    }
}
void list_iterate(List *list, void (*func)(int v))
{
    if (list == NULL || func == NULL || list->head == NULL) return;
    Node *curr = list->head;
    while (curr != NULL) 
    {
        func(curr->value);
        curr = curr->next;
    }
}