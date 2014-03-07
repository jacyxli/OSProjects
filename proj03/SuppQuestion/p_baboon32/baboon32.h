#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

#define N_BABOON 100
#define CROSS_TIME 4
#define GET_ON_ROPE 1

#define TRUE 1
#define FALSE 0

#define WEST 0
#define EAST 1
#define NEITHER 2

typedef struct{
    int id;
    int direction;
    int arrivalTime;
}Baboon;

void travel(Baboon* baboon);
void getOnRope();
void crossRope();
void perror_and_exit(char* message);
                  
