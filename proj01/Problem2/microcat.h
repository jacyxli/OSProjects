#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BUFFSIZE 512
#define STDIN 0
#define STDOUT 1
#define USAGE 77


void printfile(int fdin, int fdout);
int lenof(char* filename);
void fileNotFound(char* filename);
void inoutput(char* filename);
void printMessage(int sig);
