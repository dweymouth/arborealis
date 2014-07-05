#include <stdlib.h>
#include "stack_queue.h"

// used for both stacks and queues
typedef struct _node {
	int value;
	struct _node *next;
} _Node;

typedef struct stack {
	int size;
	_Node *top;
} Stack;

typedef struct queue {
	int size;
	_Node *front;
	_Node *back;
} Queue;

Stack *s_create() {
	Stack *stack = malloc(sizeof(Stack));
	if (!stack) {
		return NULL;
	}
	stack->size = 0;
	stack->top = NULL;
	return stack;
}

Queue *q_create() {
	Queue *queue = malloc(sizeof(Queue));
	if (!queue) {
		return NULL;
	}
	queue->size = 0;
	queue->front = NULL;
	queue->back = NULL;
	return queue;
}

int s_push(Stack *stack, int value) {
	_Node *newNode = malloc(sizeof(_Node));
	if (!newNode) {
		return 0; // memory allocation failed
	}
	newNode->value = value;
	newNode->next = stack->top;
	stack->top = newNode;
	++stack->size;
	return 1; // success
}

int q_enqueue(Queue *queue, int value) {
	_Node *newNode = malloc(sizeof(_Node));
	if (!newNode) {
		return 0; // memory allocation failed
	}
	newNode->value = value;
	if (queue->size == 0) {
		queue->front = newNode;
	} else {
		queue->back->next = newNode;
	}
	queue->back = newNode;
	++queue->size;
	return 1; // success
}

int s_pop(Stack *stack) {
	if (stack->size == 0) {
		return -1;
	}
	int value = stack->top->value;
	_Node *newTop = stack->top->next;
	free(stack->top);
	stack->top = newTop;
	--stack->size;
	return value;
}

int q_dequeue(Queue *queue) {
	if (queue->size == 0) {
		return -1;
	}
	int value = queue->front->value;
	_Node *newFront = queue->front->next;
	free(queue->front);
	queue->front = newFront;
	--queue->size;
	return value;
}

inline int s_peek(const Stack *stack) {
	return stack->top->value;
}

inline int q_peek(const Queue *queue) {
	return queue->front->value;
}

inline int s_size(const Stack *stack) {
	return stack->size;
}

inline int q_size(const Queue *queue) {
	return queue->size;
}
