COMPILERFLAGS = -g -Wall
CC = gcc
CFLAGS = $(COMPILERFLAGS)

All:	hw1b

hw1a:	hw1b.o
	$(CC) $(CFLAGS) hw1b.o -o hw1b

hw1a.o:	hw1b.c hw1b.h
	$(CC) $(CFLAGS) -c hw1b.c

clean:
	rm *.o *~ hw1b
