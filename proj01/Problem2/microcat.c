#include "microcat.h"

int main(int argc, char* argv[]){
    int fd, fout, outidx = -1, i;
	char* outfile;
	signal(SIGINT, printMessage);

	for(i = 1; i<argc; i++){
		if(!strcmp(argv[i], "+")){
			outidx = i;
			if(i+1<argc) outfile = argv[++i];
			else printMessage(USAGE);

			if((fout = open(outfile, O_TRUNC|O_WRONLY|O_CREAT,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))<0) exit(0);
			break;
		}else if(!strcmp(argv[i], "++")){
			outidx = i;
			if(i+1<argc) outfile = argv[++i];
			else printMessage(USAGE);

			if((fout = open(outfile, O_WRONLY|O_APPEND|O_CREAT,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH))<0) exit(0);
			break;
		}
	}

    //if no arguments are given
    if(argc == 1){
            printfile(STDIN, STDOUT);
    } else{		
		i = 0;
        while(++i<argc){
			/* skip the outfile and the '+'/'++' symbol*/
			if((i == outidx) || (i== outidx+1)) continue;

			if(strcmp(argv[i], "-")==0) {
				printfile(STDIN, fout);
			} else {
				if(!strcmp(outfile, argv[i])) {
					inoutput(outfile);
				}else{
		        	fd = open(argv[i], O_RDONLY);
		        	if(fd < 0) 
		            	fileNotFound(argv[i]);
		        	else
		            	printfile(fd,fout);
		        	close(fd);
				}
			}
        }
		close(fout);
   }
   return 0;
}

/*print file according to fdin to STDOUT*/
void printfile(int fdin, int fdout){
    int num;
    char buffer[BUFFSIZE];
    //while not reaching EOF
    while((num = read(fdin, buffer, BUFFSIZE))>0){
        write(fdout, buffer, num); //print the line to stdout
    }
}

/*returns the length of the char array given the pointer to the head of the char array*/
int lenof(char* filename){
    int len = 0;
    while(filename[len]!='\0'){
        len++;
    }
    return len;
}

/*print errorMessage to STDOUT when the file is not found*/
void fileNotFound(char* filename){
        write(STDOUT, "microcat: ", 10 );
        write(STDOUT, filename, lenof(filename));
        write(STDOUT, ": No such file or Directory\n", 28);
}

/*print errorMessage to STDOUT when the input is output*/
void inoutput(char* filename){
        write(STDOUT, "microcat: ", 10 );
        write(STDOUT, filename, lenof(filename));
        write(STDOUT, ": Input is output\n", 18);
}

/*print message*/
void printMessage(int sig){
	if(sig == SIGINT){
    	write(STDOUT, "Help! I think I've been shot!!\n",31);
		exit(0);
	} else if(sig == USAGE) {
		write(STDOUT, "USAGE: ./microcat [FILE]...\n",28);
	}
}
