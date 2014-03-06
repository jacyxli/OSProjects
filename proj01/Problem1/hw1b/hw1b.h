#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define BUFFSIZE 512
#define FALSE 0
#define TRUE 1

void catch_alarm( int sig );
void catch_keyboard( int sig );
int grepIntr();
unsigned int timer(unsigned int seconds);
void printUsage();
