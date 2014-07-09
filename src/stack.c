#include <stdlib.h>
#include "stack.h"

// used for both stacks and queues
typedef struct _node {
	int value;
	struct _node *next;
} _Node;

typedef struct stack {
	int size;
	_Node *top;
} Stack;

Stack *s_create() {
	Stack *stack = malloc(sizeof(Stack));
	if (!stack) {
		return NULL;
	}
	stack->size = 0;
	stack->top = NULL;
	return stack;
}

void s_destroy(Stack *stack) {
	_Node *top = stack->top;
	while (top) {
		_Node *nextTop = top->next;
		free(top);
		top = nextTop;
	}
	free(stack);
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

int s_peek(const Stack *stack) {
	if (stack->top) {
		return stack->top->value;
	}
	return -1;
}

inline int s_size(const Stack *stack) {
	return stack->size;
}
