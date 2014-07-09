#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "parse.h"
#include "run.h"

#define VERSION "0.1"

void printHelpAndExit() {
	printf("Usage: arborealis [switches] <source file>\n");
	printf("Switches:\n");
	printf("  -h, --help    Print this help screen\n");
	printf("  --version     Print version information\n");
	exit(EX_OK);
}

void printVersionAndExit() {
	printf("arborealis version %s\n", VERSION);
	printf("An Arborealis language interpreter\n");
	printf("Copyright (C) 2014 Drew Weymouth\n");
	printf("MIT License <http://opensource.org/licenses/MIT>\n");
	exit(EX_OK);
}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: arborealis <source file>\n");
		exit(EX_USAGE);
	} else if (strcmp(argv[1], "--version") == 0) {
		printVersionAndExit();
	} else if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
		printHelpAndExit();
	}

	FILE *sourceFile = fopen(argv[1], "r");
	if (!sourceFile) {
		fprintf(stdout, "Error: Could not open source file '%s'\n", argv[1]);
		exit(EX_IOERR);
	}
	Program *program = parse(sourceFile);
	fclose(sourceFile);
	if (!program) {
		// parse failed - error messages printed by parse function
		exit(EX_DATAERR);
	}

	run(program);
	exit(EX_OK);
}
