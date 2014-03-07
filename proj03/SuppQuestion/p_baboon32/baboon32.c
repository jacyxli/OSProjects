#include "baboon32.h"
int *crossers, *blocked;
sem_t *rope_mutex;
sem_t *crosser_mutex;
sem_t *direction_mutex;
sem_t *blocked_mutex;

int main(){
    Baboon baboonInfo[N_BABOON];
    srand(time(NULL));
    int i;
    Baboon b;
    pid_t pid;

    /*initialize shared variables*/
    crossers = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1,0);
    blocked = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1,0);
    if(crossers == MAP_FAILED || blocked == MAP_FAILED){ 
        perror_and_exit("Failed to create shared memory");
    }
    *crossers = 0;
    *blocked = NEITHER; 
                                             
    /*initialize semaphores*/
    sem_unlink("direction_mutex");   
    sem_unlink("rope_mutex");
    sem_unlink("crosser_mutex");
    sem_unlink("blocked_mutex");
    direction_mutex = sem_open("direction_mutex", O_CREAT|O_EXCL, S_IRUSR | S_IWUSR,1);
    rope_mutex = sem_open("rope_mutex", O_CREAT|O_EXCL, S_IRUSR | S_IWUSR, 1);
    crosser_mutex = sem_open("crosser_mutex", O_CREAT|O_EXCL, S_IRUSR | S_IWUSR,1);
    blocked_mutex = sem_open("crosser_mutex", O_CREAT|O_EXCL, S_IRUSR | S_IWUSR,1);

    /*randomly initialize baboon variables*/
    for(i = 0; i < N_BABOON;i++){
        b.id = i;
        b.direction = rand()%2;
        b.arrivalTime = rand()%8+1;
        baboonInfo[i] = b;
    }

    for(i = 0; i <N_BABOON; i++){
        pid = fork();
        if(pid<0){
            perror_and_exit("Fork Error");
        }else if(pid==0){
            break;
        }
    }

    if(pid == 0){
        //if in child process
        travel(baboonInfo+i);
    }else if(pid>0){
        //if in parent process, wait for all child process to exit
        while((pid = waitpid(-1, NULL, 0))){
            if(errno == ECHILD)
                break;
        }

        //clean up memory
        munmap(crossers, sizeof(int));
        munmap(blocked, sizeof(int));
        
        sem_unlink("direction_mutex");
        sem_unlink("rope_mutex");
        sem_unlink("crosser_mutex");
        sem_unlink("blocked_mutex");
        sem_destroy(direction_mutex);
        sem_destroy(rope_mutex);
        sem_destroy(crosser_mutex); 
        sem_destroy(blocked_mutex);
    
    }

    return 0;
}

void travel(Baboon* baboon){
    int done = FALSE;
    int resume = FALSE;
    int dir = baboon->direction;

    printf("baboon[%d] travels %s, arrives at %d sec\n", baboon->id, (dir == EAST ? "EAST" : "WEST"), baboon->arrivalTime);
    sleep(baboon->arrivalTime);
    
    while(!done){
        //test to check if there are revDir-going baboons waiting
        sem_wait(blocked_mutex);
        if(*blocked == dir || *blocked == NEITHER){ 
            //if no waiting baboons, continue updating crossers
            resume = TRUE;
        }
        //otherwise wait until the opposite direction is unblocked
        sem_post(blocked_mutex);
        
        if(resume){
            sem_wait(crosser_mutex);
            //check if there are 0 or more dir-going baboons on the rope        
            if((dir == EAST && *crossers>=0)||(dir == WEST && *crossers <=0)){
                //get on rope and increment the number of east-going crossers on the rope
                if(dir == EAST)
                    (*crossers)++;
                else
                    (*crossers)--;
                    
                getOnRope();
                
                printf("%s crossers = %d\n", (dir==EAST ? "EAST+" : "WEST-"), *crossers);
                
                if((dir == EAST && *crossers == 1)||(dir == WEST && *crossers == -1)){
                    //if the first to get on the rope, lock the direction
                    sem_wait(direction_mutex);
                }
                sem_post(crosser_mutex);

                crossRope();

                sem_wait(crosser_mutex);
                //get off rope and decrease number of east-going crossers on the rope
                if(dir == EAST)
                    (*crossers)--;
                else
                    (*crossers)++;
                printf("%s crossers = %d\n",(dir == EAST ? "EAST-" : "WEST+"), *crossers);
                
                if(*crossers == 0){
                    //if the last one to get off the rope, unlock the direction
                    sem_post(direction_mutex);
                }

                //unblock dir-going baboons since it is already travelling in the direction
                sem_wait(blocked_mutex);
                if((dir == EAST && *blocked == EAST) ||(dir == WEST && *blocked == WEST)) *blocked = NEITHER;
                sem_post(blocked_mutex);
                
                done = TRUE;
            }else{
                //if there are west-going baboons on the loop now, block EAST 
                //indicate there're waiting east-going baboons now
                sem_wait(blocked_mutex);
                *blocked = (dir == EAST ? EAST: WEST);
                sem_post(blocked_mutex);
            }
            sem_post(crosser_mutex);
        } 
    }
    exit(EXIT_SUCCESS);
}

void getOnRope(){
    /* it takes each baboon 1 sec to get on rope
     * at the meantime no other baboons can try getting on the rope
     */
    sem_wait(rope_mutex);
    sleep(GET_ON_ROPE);
    sem_post(rope_mutex);
}

void crossRope(){
    sleep(CROSS_TIME);
}

void perror_and_exit(char* message){
    perror(message);
    exit(EXIT_FAILURE);
}
