#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include "stack_queue.h"

// Program represents the instructions and current state of an Arborealis program,
// not including the tree data structure that is the execution environment.
typedef struct program {
	// The size in bytes of the program (also, the number of instructions)
	int size;
	// Array of the program instructions (last byte is NULL)
	char *instructions;
	// Stack to push the PCs for start-of-loop instructions during execution
	// The stack starts out empty after the parse stage
	Stack *jumpBack;
	// Queue of end-of-loop instruction PCs. Pre-determined during parse stage
	Queue *jumpForward;
} Program;

// Parses an Arborealis source file into a Program struct
// Prints error messages and returns NULL if the parse cannot be completed
Program *parse(FILE *);

#endif
