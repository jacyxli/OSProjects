COMPILERFLAGS = -g -Wall
CC = gcc
CFLAGS = $(COMPILERFLAGS)

All:	microcat

microcat:	microcat.o
	$(CC) $(CFLAGS) microcat.o -o microcat

microcat.o:	microcat.c microcat.h
	$(CC) $(CFLAGS) -c microcat.c

clean:
	rm *.o *~ microcat
