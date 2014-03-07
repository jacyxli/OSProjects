COMPILERFLAGS = -g -Wall 
CC = gcc
CFLAGS = $(COMPILERFLAGS)
THREADFLAGS = -lpthread -lrt

All:	baboon31

baboon31:	baboon31.o
	$(CC) $(CFLAGS) baboon31.o -o baboon31 $(THREADFLAGS)

baboon31.o:	baboon31.c baboon31.h
	$(CC) $(CFLAGS) -c baboon31.c

clean:
	rm *.o *~ baboon31 *.swp
