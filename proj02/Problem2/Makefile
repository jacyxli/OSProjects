COMPILERFLAGS = -g -Wall 
CC = gcc
CFLAGS = $(COMPILERFLAGS)

All:	shell

shell:	shell.o
	$(CC) $(CFLAGS) shell.o -o mysh

shell.o:	shell.c shell.h
	$(CC) $(CFLAGS) -c shell.c

clean:
	rm *.o *~ mysh *.swp
