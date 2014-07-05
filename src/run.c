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

// The Arborealis execution environment
typedef struct tree {
	TreeNode *root;
	TreeNode *parent;
	TreeNode *current;
} Tree;

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
	Tree *tree = malloc(sizeof(Tree));
	tree->root = tree->current = tree->parent = newNode();

	int c; // temporarily hold a character read from stdin
	// Execute the program instructions. Loop terminates when the PC
	// points to the NULL instruction.
	for (;;) {
		switch(program->instructions[program->pc]) {
		case '>': // move right if right exists
			if (tree->current->right) {
				tree->parent = tree->current;
				tree->current = tree->current->right;
			}
			break;
		case '<': // move left if left exists
			if (tree->current->left) {
				tree->parent = tree->current;
				tree->current = tree->current->left;
			}
			break;
		case '(': // left paradox function
			if (!tree->current->left) {
				tree->current->left = tree->parent;
			}
			break;
		case ')': // right paradox function
			if (!tree->current->right) {
				tree->current->right = tree->parent;
			}
			break;
		case '/': // if left doesn't exist, create and move left
			if (!tree->current->left) {
				tree->current->left = newNode();
			}
			break;
		case '\\': // if right doesn't exist, create and move right
			if (!tree->current->right) {
				tree->current->right = newNode();
			}
			break;
		case '{': // set value to 1 if left exists, 0 otherwise
			tree->current->value = (tree->current->left != NULL);
			break;
		case '}': // set value to 1 if right exists, 0 otherwise
			tree->current->value = (tree->current->right != NULL);
			break;
		case '+': // incr value at current node
			++tree->current->value;
			break;
		case '-': // decr value at current node
			--tree->current->value;
			break;
		case '!': // conditional left mover
			if (!tree->current->left) {
				tree->current->left = newNode();
				tree->current = tree->current->left;
			} else if (!tree->current->value) {
				tree->current = tree->current->left;
			} else if (!tree->current->right) {
				tree->current->right = newNode();
				tree->current = tree->current->right;
			} else if (tree->current->value) {
				tree->current = tree->current->right;
			} else break;	
			tree->parent = tree->current;
			break;
		case '?': // conditional right mover
			if (!tree->current->right) {
				tree->current->right = newNode();
				tree->current = tree->current->right;
			} else if (!tree->current->value) {
				tree->current = tree->current->right;
			} else if (!tree->current->left) {
				tree->current->left = newNode();
				tree->current = tree->current->left;
			} else if (tree->current->value) {
				tree->current = tree->current->left;
			} else break;
			tree->parent = tree->current;
			break;
		case '~': // move to root
			tree->current = tree->parent = tree->root;
			break;
		case '[': // BF loop start
			if (!tree->current->value) {
				// jump to pre-determined end-of-loop PC
				program->pc = q_dequeue(program->jumpForward);
			} else {
				// push PC for the current start-of-loop
				s_push(program->jumpBack, program->pc);
			}
			break;
		case ']': // BF loop end
			if (tree->current->value) {
				program->pc = s_peek(program->jumpBack);
			} else {
				// break out of loop and remove jump points
				q_dequeue(program->jumpForward);
				s_pop(program->jumpBack);
			}
			break;
		case '.': // put current node value to stdout
			putchar(tree->current->value);
			break;
		case ',': // set current node value = getchar (EOF = 0)
			c = getchar();
			if (c == EOF) {
				c = 0;
			}
			tree->current->value = c;
			break;
		case '\0': // end program
			return;
		}
		++program->pc;
	}
	// We never free the tree, but that's OK, because
	// once execution terminates, the interpreter terminates as well
}
