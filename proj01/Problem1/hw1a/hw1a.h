#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 512

void catch_alarm( int sig );
void catch_keyboard( int sig );
int grepIntr();
