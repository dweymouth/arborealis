#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "parse.h"
#include "stack_queue.h"

#define BUFFERSIZE 1024

inline bool isInstruction(const char ch) {
	return ch == '>' || ch == '<' || ch == '(' || ch == ')'
	    || ch == '/' || ch == '\\' || ch == '[' || ch == ']'
		|| ch == '{' || ch == '}' || ch == '+' || ch == '-'
		|| ch == '!' || ch == '?' || ch == '~' || ch == '.' || ch == ',';
}

Program *parse(FILE *sourceFile) {
	// get file size
	fseek(sourceFile, 0, SEEK_END);
	int fileSize = ftell(sourceFile);
	fseek(sourceFile, 0, SEEK_SET);
	
	Queue *jumpQueue = q_create();
	char buffer[BUFFERSIZE]; // file input buffer
	char *programBuffer = malloc(sizeof(int)*fileSize);
	int i, numRead, programSize = 0, openBraceCount = 0, lastOpenBrace;

	do {
		numRead = fread(buffer, 1, BUFFERSIZE, sourceFile);
		for (i = 0; i < numRead; i++) {
			if (!isInstruction(buffer[i])) {
				continue; // ignore non-instruction characters
			}
			programBuffer[programSize++] = buffer[i];
			if (buffer[i] == ']') {
				if (openBraceCount) { // add to end-of-loop location queue
					q_enqueue(jumpQueue, programSize-1);
					--openBraceCount;
				} else { // unmatched end-of-loop
					fprintf(stderr, "ParseError: unmatched ] (instr #%d)\n", programSize);
					free(programBuffer);
					return NULL;
				}
			} else if (buffer[i] == '[') {
				lastOpenBrace = programSize;
				++openBraceCount;
			}
		}
	} while (numRead);
	
	if (openBraceCount) {
		fprintf(stderr, "ParseError: Unmatched [ (instr #%d)\n", lastOpenBrace);
		free(programBuffer);
		return NULL;
	}
	programBuffer[programSize] = '\0'; // null-terminated program instructions
	programBuffer = realloc(programBuffer, programSize+1);
	
	Program *program = malloc(sizeof(Program));
	program->size = programSize;
	program->instructions = programBuffer;
	program->jumpForward = jumpQueue;
	return program;
}
