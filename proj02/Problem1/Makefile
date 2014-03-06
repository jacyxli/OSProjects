COMPILERFLAGS = -g -Wall 
CC = gcc
CFLAGS = $(COMPILERFLAGS)

All:	main

main:	main.o
	$(CC) $(CFLAGS) main.o -o hw02 -pthread

main.o:	main.c main.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm *.o *~ hw02
