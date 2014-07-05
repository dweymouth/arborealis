CC = gcc
CFLAGS = -iquotesrc -Wall
OBJECTS = stack_queue.o parse.o run.o main.o

arborealis: $(OBJECTS)
	@$(CC) $(CFLAGS) -o arborealis $(OBJECTS)

main.o: src/main.c
	@$(CC) $(CFLAGS) -c src/main.c

stack_queue.o: src/stack_queue.c src/stack_queue.h
	@$(CC) $(CFLAGS) -c src/stack_queue.c

parse.o: src/parse.c src/parse.h
	@$(CC) $(CFLAGS) -c src/parse.c

run.o: src/run.c src/run.h
	@$(CC) $(CFLAGS) -c src/run.c

.PHONY: clean
clean:
	@rm *.o
