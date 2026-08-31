#include <stdlib.h>
#include "stack_queue.h"

stack* creatS(int capacity)
{
    stack* S=(stack*)malloc(sizeof(stack));
    S->capacity=capacity;
    S->size=0;

    int* H=(int*)malloc(sizeof(int)*capacity);
    S->s=H;

    return S;
}
void push(stack* s,int tar)
{
    if (s->size+1>s->capacity)
        return;

    s->s[s->size]=tar;
    s->size++;
}
int pop(stack* s)
{
    if (s->size==0)
    {
        return -65536;
    } 

    return s->s[--s->size];
}
int visitS(stack* s)
{
    if (s->size==0)
    {
        return -65536;
    } 

    return s->s[s->size-1];
}
void freeS(stack* s)
{
    free(s->s);
    free(s);
}

queue* creatQ(int capacity)
{
    queue* S=(queue*)malloc(sizeof(queue));
    S->capacity=capacity;
    S->size=0;
    S->front=0;
    S->rear=0;

    int* H=(int*)malloc(sizeof(int)*capacity);
    S->q=H;

    return S;
}
void enqueue(queue* q,int tar)
{
    if (q->size==q->capacity)
    return;

    q->q[q->rear]=tar;
    
    q->size++;
    q->rear=(q->rear+1)%q->capacity;
}
int dequeue(queue* q)
{
    if (q->size==0)
    {
        return -65536;
    }

    int tmp=q->q[q->front];

    q->front=(q->front+1)%q->capacity;
    q->size--;

    return tmp;
}
int visitQ(queue* q)
{
    if (q->size==0)
    {
        return -65536;
    }

    return q->q[q->front];
}
void freeQ(queue* q)
{
    free(q->q);
    free(q);
}
