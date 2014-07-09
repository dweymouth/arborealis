#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "parse.h"
#include "stack.h"

#define BUFFERSIZE 1024

inline bool isInstruction(const char ch) {
	return ch == '>' || ch == '<' || ch == '(' || ch == ')'
	    || ch == '/' || ch == '\\' || ch == '[' || ch == ']'
		|| ch == '{' || ch == '}' || ch == '+' || ch == '-'
		|| ch == '!' || ch == '?' || ch == '~' || ch == '.' || ch == ',';
}

inline int fileSize(FILE *file) {
	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	fseek(file, 0, SEEK_SET);
	return size;
}

inline Program *createProgram(int size, char *instructions, Hashtable *jumpTable) {
	Program program = malloc(sizeof(Program));
	program->size = size;
	program->instructions = instructions;
	program->jumpTable = jumpTable;
	return program;
}

Program *parse(FILE *sourceFile) {
	Stack *loopContext = s_create();
	Hashtable *jumpTable = ht_create(0);

	// allocate an array the same size as source file to hold instructions
	char *programBuffer = malloc(sizeof(int)*fileSize(sourceFile));
	
	char buffer[BUFFERSIZE]; // file input buffer
	int i, numRead, programSize = 0;
	do {
		// read next chunk of source file
		numRead = fread(buffer, 1, BUFFERSIZE, sourceFile);
		for (i = 0; i < numRead; i++) {
			if (!isInstruction(buffer[i])) {
				continue; // ignore non-instruction characters
			}
			programBuffer[programSize++] = buffer[i];
			if (buffer[i] == '[') {
				s_push(loopContext, programSize - 1);
			} else if (buffer[i] == ']') {
				if (s_size(loopContext) > 0) { // add to lookup table
					ht_add(jumpTable, s_pop(loopContext), programSize - 1);
				} else { // unmatched end-of-loop
					fprintf(stderr, "ParseError: unmatched ] (instr #%d)\n", programSize);
					free(programBuffer);
					s_destroy(loopContext);
					return NULL;
				}
			}
		}
	} while (numRead);
	
	if (s_size(loopContext) > 0) {
		fprintf(stderr, "ParseError: Unmatched [ (instr #%d)\n", s_pop(loopContext) + 1);
		free(programBuffer);
		s_destroy(loopContext);
		return NULL;
	}

	s_destroy(loopContext);
	programBuffer[programSize] = '\0'; // null-terminated program instructions
	// if source file had comments, there's un-needed space in programBuffer
	programBuffer = realloc(programBuffer, programSize+1);
	
	return createProgram(programSize, programBuffer, jumpTable);
}
