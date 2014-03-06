#include "hw1a.h"

int main(int argc, char* argv[]){
	int before, after; 
	// register signal handler for alarm and keyboard interrupts
	signal(SIGALRM, catch_alarm); 
	signal(SIGINT, catch_keyboard);

	/*store the total number of interrupts before setting the alarm*/
	before = grepIntr(); 
    alarm(1);  /* timer will pop in 1 seconds */
	pause();
	/*store the total number of interrupts after setting the alarm*/
	after = grepIntr();

	printf("%d\n", after-before);
	printf("Total no. of interrupts since boot time: %d\n", after);
    return 0;
}

void catch_alarm( int sig ) {
    printf("Signal caught: SIGALRM = %d\n", sig);
	printf("No. of interrupts since last 1 sec: ");
}

void catch_keyboard( int sig ) {
    printf( "Signal caught: SIGINT = %d\n", sig);
	printf("No. of interrupts before program stops: ");
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

