#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include "hashtable.h"

// Program represents the instructions and current state of an Arborealis program,
// not including the tree data structure that is the execution environment.
typedef struct program {
	// The size in bytes of the program (also, the number of instructions)
	int size;
	// Array of the program instructions (last byte is NULL)
	char *instructions;
	// Lookup table for PC of matching end loop ']' given PC of start loop '['
	Hashtable *jumpTable;
} Program;

// Parses an Arborealis source file into a Program struct
// Prints error messages and returns NULL if the parse cannot be completed
Program *parse(FILE *);

#endif
