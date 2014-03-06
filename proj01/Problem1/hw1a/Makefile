COMPILERFLAGS = -g -Wall
CC = gcc
CFLAGS = $(COMPILERFLAGS)

All:	hw1a

hw1a:	hw1a.o
	$(CC) $(CFLAGS) hw1a.o -o hw1a

hw1a.o:	hw1a.c hw1a.h
	$(CC) $(CFLAGS) -c hw1a.c

clean:
	rm *.o *~ hw1a
