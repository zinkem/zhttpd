default: all

clean:
	rm *.o
	rm zhttpd

dictionary.o: dictionary.c
	gcc -g -c dictionary.c

socket.o: socket.c
	gcc -g -c socket.c

zstring.o: zstring.c
	gcc -g -c zstring.c

all: dictionary.o socket.o zstring.o
	gcc -g zhttpd.c dictionary.o socket.o zstring.o -o zhttpd