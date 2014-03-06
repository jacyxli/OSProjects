#ifndef _REENTRANT
	#define _REENTRANT
#endif
#define N 5
#define MAX_SLEEP 5

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

struct tuple{
	int i;
	int j;
};

void handle_error(char* msg);
void *addValue(void *arg);
void *multValue(void *arg);
void *maxRowSum(void *arg);
void printMatrix(int M[N][N]);
