COMPILERFLAGS = -g -Wall 
CC = gcc
CFLAGS = $(COMPILERFLAGS)
THREADFLAGS = -lpthread -lrt

All:	baboon32

baboon32:	baboon32.o
	$(CC) $(CFLAGS) baboon32.o -o baboon32 $(THREADFLAGS)

baboon32.o:	baboon32.c baboon32.h
	$(CC) $(CFLAGS) -c baboon32.c

clean:
	rm *.o *~ baboon32 *.swp
