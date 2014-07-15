CC = gcc
CFLAGS = -iquotesrc -Wall
OBJECTS = stack.o hashtable.o parse.o run.o main.o

arborealis: $(OBJECTS)
	@$(CC) $(CFLAGS) -o arborealis $(OBJECTS)

main.o: src/main.c
	@$(CC) $(CFLAGS) -c src/main.c

stack.o: src/stack.c src/stack.h
	@$(CC) $(CFLAGS) -c src/stack.c

hashtable.o: src/hashtable.c src/hashtable.h
	@$(CC) $(CFLAGS) -c src/hashtable.c

parse.o: src/parse.c src/parse.h
	@$(CC) $(CFLAGS) -c src/parse.c

run.o: src/run.c src/run.h
	@$(CC) $(CFLAGS) -c src/run.c

.PHONY: clean
clean:
	@rm -f *.o
