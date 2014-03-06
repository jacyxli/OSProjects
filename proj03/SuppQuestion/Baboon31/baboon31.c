#include "baboon31.h"

/* number of crossers on the rope
 * if positive, travelling east
 * if negative, travelling west
 */
int crossers;

/*semaphores*/
sem_t rope_mutex;
sem_t crosser_mutex;
sem_t direction_mutex;

int main(){
    Baboon baboonInfo[N_BABOON];
    pthread_t baboons[N_BABOON];
    srand(time(NULL));
    int i;
    Baboon b;
    
    /*initialize semaphores*/
    sem_init(&direction_mutex, 0, 1);
    sem_init(&rope_mutex, 0, 1);
    sem_init(&crosser_mutex, 0, 1);
    
    /*randomly initialize baboon variables*/
    for(i = 0; i < N_BABOON;i++){
        b.id = i;
        b.direction = rand()%2;
        b.arrivalTime = rand()%8+1;
        baboonInfo[i] = b;
    }

	/*initializing threads*/
    for(i = 0; i < N_BABOON; i++){
        if(baboonInfo[i].direction == EAST){
            pthread_create(&baboons[i], NULL, travelEast, (void *)(baboonInfo+i));
        }else{
            pthread_create(&baboons[i], NULL, travelWest, (void *)(baboonInfo+i));
        }
    }

	/*join all the threads*/
    for(i = 0; i < N_BABOON;i++){
        pthread_join(baboons[i], NULL);
    }

    return 0;
}

void getOnRope(){
    sem_wait(&rope_mutex);
    sleep(GET_ON_ROPE);
    sem_post(&rope_mutex);
}

void* travelEast(void* args){
    Baboon *baboon = (Baboon *)args;
    int done = FALSE;

    printf("baboon[%d] sleeps %d secs\n", baboon->id, baboon->arrivalTime);
    sleep(baboon->arrivalTime);

    while(!done){
        sem_wait(&crosser_mutex);
        //if there are east-going baboons on the rope
        if(crossers>=0){
            //get on the rope and increment no of crossers on the rope
            getOnRope();
            crossers++;
            printf("EASTi+ ncrossers = %d\n", crossers);
            //if the first to get on the rope
            if(crossers == 1){ sem_wait(&direction_mutex); }
            sem_post(&crosser_mutex);

            //crossing 4 secs
            sleep(CROSS_TIME);

            sem_wait(&crosser_mutex);
            //get off the loop
            crossers--;
            printf("EAST- ncrossers = %d\n", crossers);
            //if the last one to get off the loop
            // unlock the direction
            if(crossers == 0){ sem_post(&direction_mutex);}
            done = TRUE;
        }
        sem_post(&crosser_mutex);
    }
    return NULL;
}

void* travelWest(void* args){
    Baboon *baboon = (Baboon *)args;
    int done = FALSE;

    printf("baboon[%d] sleeps %d secs\n", baboon->id, baboon->arrivalTime);
    sleep(baboon->arrivalTime);

    while(!done){
        sem_wait(&crosser_mutex);
        //if there are west-going baboons on the rope
        if(crossers<=0){
            getOnRope();
			//increment the number of baboons on the rope (west thus negative)
            crossers--;
            printf("WEST- ncrossers = %d\n", crossers);
            //if the first to get on the rope
            if(crossers == -1){ sem_wait(&direction_mutex);}
            sem_post(&crosser_mutex);
            
			//crossing 4 secs    
            sleep(CROSS_TIME);

            sem_wait(&crosser_mutex);
            //get off the loop
            crossers++;
            printf("WEST+ ncrossers = %d\n", crossers);
            //if the last one to get off the loop
            // unlock the direction
            if(crossers == 0){ sem_post(&direction_mutex);}
            done = TRUE;
        }
        sem_post(&crosser_mutex);
    }
    return NULL;
}
