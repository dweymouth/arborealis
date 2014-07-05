CC = gcc
CFLAGS = -Wall
OBJECTS = stack_queue.o parse.o run.o main.o

arborealis: $(OBJECTS)
	@$(CC) $(CFLAGS) -o arborealis $(OBJECTS)

main.o: main.c
	@$(CC) $(CFLAGS) -c main.c

stack_queue.o: stack_queue.c stack_queue.h
	@$(CC) $(CFLAGS) -c stack_queue.c

parse.o: parse.c parse.h
	@$(CC) $(CFLAGS) -c parse.c

run.o: run.c run.h
	@$(CC) $(CFLAGS) -c run.c

.PHONY: clean
clean:
	@rm *.o
