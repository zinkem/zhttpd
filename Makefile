default: all

clean:
	rm *.o
	rm zhttpd

dictionary.o: dictionary.c
	gcc -c dictionary.c

socket.o: socket.c
	gcc -c socket.c

zstring.o: zstring.c
	gcc -c zstring.c

all: dictionary.o socket.o zstring.o
	gcc zhttpd.c dictionary.o socket.o zstring.o -o zhttpd