
#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

typedef struct 
{
    int* s;
    int size;
    int capacity;
}stack;

typedef struct 
{
    int* q;
    int front;
    int rear;
    int size;
    int capacity;
}queue;

stack* creatS(int capacity);
void push(stack* s,int tar);
int pop(stack* s);
int visitS(stack* s);
void freeS(stack* s);

queue* creatQ(int capacity);
void enqueue(queue* q,int tar);
int dequeue(queue* q);
int visitQ(queue* q);
void freeQ(queue* q);

#endif // STACK_QUEUE_H
