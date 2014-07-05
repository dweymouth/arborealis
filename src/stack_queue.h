#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

typedef struct stack Stack;

Stack *s_create();

int s_push(Stack *, int);

int s_pop(Stack *);

int s_peek(const Stack *);

int s_size(const Stack *);


struct queue;
typedef struct queue Queue;

Queue *q_create();

int q_enqueue(Queue *, int);

int q_dequeue(Queue *);

int q_peek(const Queue *);

int q_size(const Queue *);

#endif
