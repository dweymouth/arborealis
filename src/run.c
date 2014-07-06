#include <stdlib.h>
#include <stdio.h>

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
	Queue *jumpForward = program->jumpForward;
	Stack *jumpBack = program->jumpBack;

	register int pc = 0; // program counter
	int c; // temporarily hold a character read from stdin

	// Execute the program instructions. Loop terminates when the PC
	// points to the NULL instruction.
	for (;;) {
		switch(instructions[pc]) {
		case '>': // move right if right exists
			if (current->right) {
				parent = current;
				current = current->right;
			}
			break;
		case '<': // move left if left exists
			if (current->left) {
				parent = current;
				current = current->left;
			}
			break;
		case '(': // left paradox function
			if (!current->left) {
				current->left = parent;
			}
			break;
		case ')': // right paradox function
			if (!current->right) {
				current->right = parent;
			}
			break;
		case '/': // if left doesn't exist, create and move left
			if (!current->left) {
				current->left = newNode();
			}
			break;
		case '\\': // if right doesn't exist, create and move right
			if (!current->right) {
				current->right = newNode();
			}
			break;
		case '{': // set value to 1 if left exists, 0 otherwise
			current->value = (current->left != NULL);
			break;
		case '}': // set value to 1 if right exists, 0 otherwise
			current->value = (current->right != NULL);
			break;
		case '+': // incr value at current node
			++current->value;
			break;
		case '-': // decr value at current node
			--current->value;
			break;
		case '!': // conditional left mover
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
		case '?': // conditional right mover
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
		case '~': // move to root
			current = parent = root;
			break;
		case '[': // BF loop start
			if (!current->value) {
				// jump to pre-determined end-of-loop PC
				pc = q_dequeue(jumpForward);
			} else {
				// push PC for the current start-of-loop
				s_push(jumpBack, pc);
			}
			break;
		case ']': // BF loop end
			if (current->value) {
				pc = s_peek(jumpBack);
			} else {
				// break out of loop and remove jump points
				q_dequeue(jumpForward);
				s_pop(jumpBack);
			}
			break;
		case '.': // put current node value to stdout
			putchar(current->value);
			break;
		case ',': // set current node value = getchar (EOF = 0)
			c = getchar();
			if (c == EOF) {
				c = 0;
			}
			current->value = c;
			break;
		case '\0': // end program
			return;
		}
		++pc;
	}
	// We never free the tree, but that's OK, because
	// once execution terminates, the interpreter terminates as well
}
