#include "baboon31.h"

/* number of crossers on the rope
 * if positive, travelling east
 * if negative, travelling west
 */
int *crossers;
/*semaphores*/
sem_t *rp_mtx;
sem_t *crs_mtx;
sem_t *dir_mtx;

int main(){
    Baboon baboonInfo[N_BABOON];
    srand(time(NULL));
    int i;
    Baboon b;
    pid_t pid;

    /* initialize shared variables in shared memory */
    
    crossers = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1,0);
    if(crossers == MAP_FAILED){
        perror("error mapping the variable");
        exit(EXIT_FAILURE);
    }
    *crossers = 0;
    printf("crossers initialized on shared memory to be %d\n", crossers[0]);

    /*initialize semaphores*/
    sem_unlink("dir_mtx");
    sem_unlink("rp_mtx");
    sem_unlink("crs_mtx");
    dir_mtx = sem_open("dir_mtx", O_CREAT|O_EXCL, S_IRUSR | S_IWUSR,1);
    rp_mtx = sem_open("rp_mtx", O_CREAT|O_EXCL, S_IRUSR | S_IWUSR, 1);
    crs_mtx = sem_open("crs_mtx", O_CREAT|O_EXCL, S_IRUSR | S_IWUSR,1);

    /*randomly initialize baboon variables*/
    for(i = 0; i < N_BABOON;i++){
        b.id = i;
        b.direction = rand()%2;
        b.arrivalTime = rand()%8+1;
        baboonInfo[i] = b;
    }

    for(i = 0; i < N_BABOON; i++){
        pid = fork();
        if(pid<0){
            printf("Fork Error.\n");
        }else if(pid == 0){
            break;
        }
    }
    
    if(pid ==0){
        //child process
        travel(baboonInfo+i);
    }else if(pid > 0){
        //if parent, wait for all children to exit
        while((pid = waitpid(-1, NULL,0))){
            if(errno == ECHILD)
                break;
        }
        
        munmap(crossers, sizeof(int));

        sem_unlink("dir_mtx");
        sem_unlink("rp_mtx");
        sem_unlink("crs_mtx");

        sem_destroy(dir_mtx);
        sem_destroy(rp_mtx);
        sem_destroy(crs_mtx);    
    } 

    return 0;
}

void travel(Baboon* baboon){
    int done = FALSE;
    int dir = baboon->direction;

    printf("baboon[%d] sleeps %d secs\n", baboon->id, baboon->arrivalTime);
    sleep(baboon->arrivalTime);

    while(!done){
        sem_wait(crs_mtx);
        //if there are east-going baboons on the rope
        if((dir == EAST && *crossers>=0) || (dir == WEST && *crossers <=0)){
            //get on the rope and increment no of crossers on the rope
            getOnRope();
            if(dir == EAST) (*crossers)++;
            else (*crossers)--;

            printf("%s crossers = %d\n", (dir == EAST? "EAST+" : "WEST-"), *crossers);
            //if the first to get on the rope
            if((dir == EAST && *crossers == 1)||(dir == WEST && *crossers == -1)){ 
                sem_wait(dir_mtx); 
            }
            sem_post(crs_mtx);

            //crossing 4 secs
            crossRope();

            sem_wait(crs_mtx);
            //get off the loop
            if(dir == EAST) (*crossers)--;
            else (*crossers)++;
            printf("%s crossers = %d\n", (dir == EAST? "EAST-":"WEST+"),*crossers);
            //if the last one to get off the loop
            // unlock the direction
            if(*crossers == 0){ sem_post(dir_mtx);}
            done = TRUE;
        }
        sem_post(crs_mtx);
    }
    exit(EXIT_SUCCESS);
}

void getOnRope(){
    sem_wait(rp_mtx);
    sleep(GET_ON_ROPE);
    sem_post(rp_mtx);
}

void crossRope(){
    sleep(CROSS_TIME);
}
