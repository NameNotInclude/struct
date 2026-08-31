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
    h.head = NULL;
    h.tail = NULL;
    return h;
}

void list_free(List *list)
{
    Node *cur = list->head;
    while (cur != NULL)
    {
        Node *next = cur->next;
        free(cur);
        cur = next;
    }
    list->head = NULL;
    list->tail = NULL;
}

void list_append(List *list, int v)
{
    Node *newnode = (Node*)malloc(sizeof(Node));
    newnode->value = v;
    newnode->next = NULL;
    if (list->tail == NULL)
    {
        list->head = newnode;
        list->tail = newnode;
    }
    else
    {
        list->tail->next = newnode;
        list->tail = newnode;
    }
}

void list_insert(List *list, int v)
{
    Node *newnode = (Node*)malloc(sizeof(Node));
    newnode->value = v;
    newnode->next = list->head;
    if (list->tail == NULL)
        list->tail = newnode;
    list->head = newnode;
}

void list_set(List *list, int index, int v)
{
    int i = 0;
    Node *check = list->head;
    while (i < index && check != NULL)
    {
        check = check->next;
        i++;
    }
    if (check == NULL)
        return;
    check->value = v;
}

int list_get(List *list, int index)
{
    int i = 0;
    Node *check = list->head;
    while (check != NULL && i < index)
    {
        check = check->next;
        i++;
    }
    return check != NULL ? check->value : 0;
}

int list_size(List *list)
{
    int i = 0;
    Node *check = list->head;
    while (check != NULL)
    {
        i++;
        check = check->next;
    }
    return i;
}

int list_find(List *list, int v)
{
    int i = 0;
    Node *check = list->head;
    while (check != NULL && check->value != v)
    {
        check = check->next;
        i++;
    }
    return check == NULL ? -1 : i;
}

void list_remove(List *list, int v)
{
    if (list->head == NULL)
        return;

    if (list->head->value == v)
    {
        Node *tmp = list->head;
        list->head = list->head->next;
        if (list->tail == tmp)
            list->tail = NULL;
        free(tmp);
        return;
    }

    Node *prev = list->head;
    Node *curr = list->head->next;
    while (curr != NULL && curr->value != v)
    {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL)
        return;

    prev->next = curr->next;
    if (curr == list->tail)
        list->tail = prev;
    free(curr);
}

void list_iterate(List *list, void (*func)(int v))
{
    if (list == NULL || func == NULL || list->head == NULL)
        return;
    Node *curr = list->head;
    while (curr != NULL)
    {
        func(curr->value);
        curr = curr->next;
    }
}
