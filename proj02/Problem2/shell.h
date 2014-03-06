#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0
#define MAXLEN 256
#define NARGS 64
#define HISTDEFAULT 5

void printPrompt();
void readCmdLine(char** cmdline);
void freeAll();
int isBuildinCmd(int nargs, char** args);
void execute(int nargs, char** args);
