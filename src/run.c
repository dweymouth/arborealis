#include <stdlib.h>
#include <stdio.h>

#include "stack.h"
#include "parse.h"
#include "run.h"

// A single node in the Arborealis tree
typedef struct treenode {
	struct treenode *left;
	struct treenode *right;
	unsigned char value;
} TreeNode;

// Return a pointer to a newly initialized TreeNode
TreeNode *newNode() {
	TreeNode *theNode = malloc(sizeof(TreeNode));
	theNode->value = 0;
	theNode->left = NULL;
	theNode->right = NULL;
	return theNode;
}

void run(Program *program) {
	// Initialize the tree
	TreeNode *root, *parent, *current;
	root = current = parent = newNode();

	// Localize pointers for optimization
	char *instructions = program->instructions;
	Hashtable *jumpTable = program->jumpTable;

	// Stack to keep track of current nested loop
	Stack *loopContext = s_create();

	register int pc = 0, jumpBack; // program counter, PC of current start loop
	int c; // temporarily hold a character read from stdin

	// Execute the program instructions. Loop terminates when the PC
	// points to the NULL instruction.
	for (;;) {
		switch(instructions[pc]) {
		case MOVE_LEFT: // move left if right exists
			if (current->left) {
				parent = current;
				current = current->left;
			}
			break;
		case MOVE_RIGHT: // move right if left exists
			if (current->right) {
				parent = current;
				current = current->right;
			}
			break;
		case MOVE_ROOT: // move to root
			current = parent = root;
			break;
		case PARADOX_LEFT: // left paradox function
			if (!current->left) {
				current->left = parent;
			}
			break;
		case PARADOX_RIGHT: // right paradox function
			if (!current->right) {
				current->right = parent;
			}
			break;
		case CREATE_LEFT: // if left doesn't exist, create and move left
			if (!current->left) {
				current->left = newNode();
			}
			break;
		case CREATE_RIGHT: // if right doesn't exist, create and move right
			if (!current->right) {
				current->right = newNode();
			}
			break;
		case IF_LEFT: // set value to 1 if left exists, 0 otherwise
			current->value = (current->left != NULL);
			break;
		case IF_RIGHT: // set value to 1 if right exists, 0 otherwise
			current->value = (current->right != NULL);
			break;
		case INCR: // incr value at current node
			++current->value;
			break;
		case DECR: // decr value at current node
			--current->value;
			break;
		case CONDITIONAL_LEFT: // conditional left mover
			if (!current->left) {
				current->left = newNode();
				current = current->left;
			} else if (!current->value) {
				current = current->left;
			} else if (!current->right) {
				current->right = newNode();
				current = current->right;
			} else if (current->value) {
				current = current->right;
			} else break;	
			parent = current;
			break;
		case CONDITIONAL_RIGHT: // conditional right mover
			if (!current->right) {
				current->right = newNode();
				current = current->right;
			} else if (!current->value) {
				current = current->right;
			} else if (!current->left) {
				current->left = newNode();
				current = current->left;
			} else if (current->value) {
				current = current->left;
			} else break;
			parent = current;
			break;
		case LOOP_START: // BF loop start
			if (!current->value) { // skipping a loop
				// jump to pre-determined end-of-loop PC
				pc = ht_lookup(jumpTable, pc);
			} else { // entering a loop
				// push PC for the current start-of-loop
				s_push(loopContext, pc);
				jumpBack = pc;
			}
			break;
		case LOOP_END: // BF loop end
			if (current->value) { // continuing a loop
				pc = jumpBack;
			} else { // exiting a loop
				s_pop(loopContext);
				jumpBack = s_peek(loopContext);
			}
			break;
		case PUTCHAR: // put current node value to stdout
			putchar(current->value);
			break;
		case GETCHAR: // set current node value = getchar (EOF = 0)
			c = getchar();
			if (c == EOF) {
				c = 0;
			}
			current->value = c;
			break;
		case END_PROGRAM:
			return;
		}
		++pc;
	}
	// We never free the tree, but that's OK, because
	// once execution terminates, the interpreter terminates as well
}
