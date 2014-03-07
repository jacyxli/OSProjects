#include "baboon32.h"
int crossers, blocked = NEITHER;
sem_t rope_mutex;
sem_t crosser_mutex;
sem_t direction_mutex;
sem_t blocked_mutex;

int main(){
    Baboon baboonInfo[N_BABOON];
    pthread_t baboons[N_BABOON];
    srand(time(NULL));
    int i;
    Baboon b;
    
    /*initialize semaphores*/
    sem_init(&blocked_mutex, 0, 1);    
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

    for(i = 0; i < N_BABOON; i++){
            pthread_create(&baboons[i], NULL, travel, (void *)(baboonInfo+i));
    }

    for(i = 0; i < N_BABOON;i++){
        pthread_join(baboons[i], NULL);
    }

    return 0;
}


void getOnRope(){
    /* it takes each baboon 1 sec to get on rope
     * at the meantime no other baboons can try getting on the rope
     */
    sem_wait(&rope_mutex);
    sleep(GET_ON_ROPE);
    sem_post(&rope_mutex);
}

void* travel(void* args){
    Baboon *baboon = (Baboon *)args;
    int done = FALSE;
    int resume = FALSE;
    int dir = baboon->direction;

    printf("baboon[%d] travels %s, arrives at %d sec\n", baboon->id, (dir == EAST ? "EAST" : "WEST"), baboon->arrivalTime);

    sleep(baboon->arrivalTime);
    
    while(!done){
        //test to check if there are revDir-going baboons waiting
        sem_wait(&blocked_mutex);
        if(blocked == dir || blocked == NEITHER){ 
            //if no waiting baboons, continue updating crossers
            resume = TRUE;
        }
        //otherwise wait until WEST is unblocked
        sem_post(&blocked_mutex);
        
        if(resume){
            sem_wait(&crosser_mutex);
            //check if there are 0 or more dir-going baboons on the rope        
            if((dir == EAST && crossers>=0)||(dir == WEST && crossers <=0)){
                //get on rope and increment the number of east-going crossers on the rope
                if(dir == EAST)
                    crossers++;
                else
                    crossers--;
                    
                getOnRope();
                
                printf("%s crossers = %d\n", (dir==EAST ? "EAST+" : "WEST-"), crossers);
                
                if(abs(crossers) == 1){
                    //if the first to get on the rope, lock the direction
                    sem_wait(&direction_mutex);
                }
                sem_post(&crosser_mutex);

                //crossing 4 secs
                sleep(CROSS_TIME);

                sem_wait(&crosser_mutex);
                //get off rope and decrease number of east-going crossers on the rope
                if(dir == EAST)
                    crossers--;
                else
                    crossers++;
                printf("%s crossers = %d\n",(dir == EAST ? "EAST-" : "WEST+"), crossers);
                
                if(crossers == 0){
                    //if the last one to get off the rope, unlock the direction
                    sem_post(&direction_mutex);
                }

                //unblock dir-going baboons since it is already travelling in the direction
                sem_wait(&blocked_mutex);
                if((dir == EAST && blocked == EAST) ||(dir == WEST && blocked == WEST)) blocked = NEITHER;
                sem_post(&blocked_mutex);
                
                done = TRUE;
            }else{
                //if there are west-going baboons on the loop now, block EAST 
                //indicate there're waiting east-going baboons now
                sem_wait(&blocked_mutex);
                blocked = (dir == EAST ? EAST: WEST);
                sem_post(&blocked_mutex);
            }
            sem_post(&crosser_mutex);
        } 
    }
    return NULL;
}
