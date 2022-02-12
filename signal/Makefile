all: release debug

CC = gcc

release: try1.c
	$(CC) -o release -O try1.c
debug: try1.c
	$(CC) -DDEBUG -g -o debug try1.c

clean:
	rm -f *.o release debug
