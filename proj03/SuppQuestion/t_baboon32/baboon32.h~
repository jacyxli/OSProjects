#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <pthread.h>

#define N_BABOON 100
#define CROSS_TIME 4
#define GET_ON_ROPE 1

#define TRUE 1
#define FALSE 0

#define NEITHER 2
#define WEST 0
#define EAST 1

typedef struct{
        int id;
            int direction;
                int arrivalTime;
}Baboon;

void getOnRope();
void* travel(void* args);
