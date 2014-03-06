#include "hw1b.h"

int keyboard = FALSE;

int main(int argc, char* argv[]){
	int before, after, i, secs;
	int sflag = FALSE; 
	// register signal handler for alarm and keyboard interrupts
	signal(SIGALRM, catch_alarm); 
	signal(SIGINT, catch_keyboard);
	
	/*check if -s flag is used, if so, read the next input as the seconds to pause*/
	for(i = 1; i<argc; i++){
		if(strcmp(argv[i],"-s")==0){
			sflag = TRUE;
			if(i+1<argc && sscanf(argv[i+1], "%d", &secs)) {
				break;
			}else{
				printUsage();
				exit(0);
			} 
		}	
	}

	/*store the total number of interrupts before setting the alarm*/
	before = grepIntr(); 
    timer((sflag ? secs : 1));  /* timer will pop in 1 seconds */
	pause();
	/*store the total number of interrupts after setting the alarm*/
	after = grepIntr();

	if(!keyboard) printf("No. of interrupts since last %d sec: ", (sflag ? secs : 1));
	printf("%d\n", after-before);
	printf("Total no. of interrupts since boot time: %d\n", after);
    return 0;
}

void catch_alarm( int sig ) {
    printf("Signal caught: SIGALRM = %d\n", sig);
}

void catch_keyboard( int sig ) {
    printf( "Signal caught: SIGINT = %d\n", sig);
	printf("No. of interrupts before program stops: ");
	keyboard = TRUE;
}

int grepIntr(){
	char buffer[BUFFSIZE];
	int fd;
	int nintr; /*total number of interrupts since boot time*/
	char *tok;
	
	fd = open("/proc/stat", O_RDONLY);
	if(fd >= 0) {
		read(fd, &buffer, BUFFSIZE); /*read process stats*/
		tok = strtok(buffer, " \n");/*tokenize the buffer by white space and newline*/
		/*iterate through the tokens until 'intr' is found*/
		while(tok != NULL && strcmp(tok, "intr")){ 
			tok = strtok(NULL, " \n");		
		}
		tok = strtok(NULL, " \n");		
		sscanf(tok, "%d", &nintr); /*store the read integer value to nintr*/
	} else {
		/*if fail to read the file, print error and exit*/
		printf("Problem reading the process information\n");
		exit(0);	
	}
	close(fd);
	return nintr;
}

unsigned int timer (unsigned int seconds){
	struct itimerval old, new;
	new.it_interval.tv_usec = 0;
 	new.it_interval.tv_sec = 0;
 	new.it_value.tv_usec = 0;
 	new.it_value.tv_sec = (long int) seconds;
    if (setitimer (ITIMER_REAL, &new, &old) < 0)
    	return 0;
	else
		return old.it_value.tv_sec;
}

void printUsage(){
	printf("USAGE:\t ./hw1b [OPTIONS]\n");
	printf("OPTIONS:\t -s seconds\t prints the number of interrupts in the previous seconds\n");
}

