all: linkedlist.o

linkedlist.o: linkedlist.c
	cc -std=c99 -o linkedlist linkedlist.c
debug: linkedlist.c
	cc -std=c99 -g -DDEBUG -o linkedlist linkedlist.c

clean:
	rm linkedlist
