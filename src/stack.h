#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

typedef struct stack Stack;

Stack *s_create();

void s_destroy(Stack *);

int s_push(Stack *, int);

int s_pop(Stack *);

int s_peek(const Stack *);

int s_size(const Stack *);

#endif
