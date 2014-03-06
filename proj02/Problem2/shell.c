#include "shell.h"
char *cmdline;
int histChange = FALSE;
char **hist;
int histsize = HISTDEFAULT;

int main(int argc, char **argv){
    char *tok;
    int mem, i, cur = 0;
    char *args[NARGS];
    int nargs;
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);
    sigprocmask(SIG_BLOCK, &mask, NULL); /*block keyboard interrupt*/

    //hist = (char**)malloc(HISTDEFAULT*sizeof(char *));
    while(TRUE){
        printPrompt();
      /*  
        if(histChange){
            hist = (char**)realloc(hist, histsize*sizeof(char*));
            histChange = FALSE;
        }
	*/

        //read command lines 
        mem = MAXLEN; 
        cmdline = malloc(mem);
        if(fgets(cmdline, mem, stdin) == NULL) {
		perror("invalid input, logout\n");
		freeAll();
		exit(0);
	}
        while(cmdline[mem-1]!='\n' && cmdline[mem-1]!=NULL){
            mem = strlen(cmdline);
            cmdline = realloc(cmdline, mem+MAXLEN);
            fgets(cmdline+mem-1, MAXLEN, stdin);
        }

 
        //parse
        i = 0;
        if((tok = strtok(cmdline, " \n\t"))==NULL) continue;
        
        //add to history
        /*hist[cur++] = (char *)malloc(strlen(cmdline)*sizeof(char));
        strcpy(hist[cur], cmdline);
        free(hist[cur-histsize]); */        

        while(tok!=NULL && i < 64){
            args[i++] = tok;
            tok = strtok(NULL, " \n\t");
        }
        nargs = i;
	
        //execute
        execute(nargs, args);
        freeAll();  
    }

    return 0;
}

void printPrompt(){
    char cwd[MAXLEN];
    if(getcwd(cwd, MAXLEN) == NULL){
        perror("getcwd() error");
    }
    printf(">> %s $ ", cwd);
}

void freeAll(){
    free(cmdline);
    //free(hist);
}

int isBuildinCmd(int nargs, char**args){
    if(strcmp(args[0], "exit") == 0){
        if(nargs == 1) {
            freeAll();
            exit(0);
        } else {
            printf("Syntax Error:\t enter 'exit' to quit\n");
        }
        return TRUE;
    }

    return FALSE;
}

void execute(int nargs, char** args){
    pid_t pid;
    int status;
    if(!isBuildinCmd(nargs, args)){
    pid = fork();
    if(pid == -1){
        perror("fork failed");
        exit(1);
    }else if(pid == 0){
        //we are in child process
        if(execvp(args[0], args)<0){
            perror("Exec failed\n");
        }
        exit(EXIT_SUCCESS);
    }else{
        //wait for completion
        while(wait(&status)!=pid);
    }
    }

}
