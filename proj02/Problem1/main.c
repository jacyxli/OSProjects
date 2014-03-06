#include "main.h"


int A[N][N];
int B[N][N];
int C[N][N];
unsigned int seeds[N][N];
int MAX_ROW_SUM;
pthread_mutex_t mutex;

int main(int argc, char **argv){
	int i, j, s;
	pthread_t tid[N][N];
	struct tuple tp[N][N];

	MAX_ROW_SUM = 0; //initialize MAX_ROW_SUM to be 0
	pthread_mutex_init(&mutex, NULL);
	
	for(i = 0; i<N; i++){
		for(j = 0; j <N; j++){
			struct timeval now;
			gettimeofday(&now, NULL);
			seeds[i][j] = now.tv_usec+(i+7)*(j+11); //make sure each seed get distinct values
		}	
	}	
	// create N by N threads
	for(i = 0; i<N; i++){
		for(j = 0; j<N; j++){
			tp[i][j].i = i;
			tp[i][j].j = j;
		
			s = pthread_create(&tid[i][j], NULL, &addValue, &tp[i][j]);
			if(s != 0) handle_error("pthread_create failure"); 
		}
	}

	// join the threads and handle errors
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			s = pthread_join(tid[i][j], NULL);
			if(s!=0) handle_error("pthread_join failture");
		}
	}


	//print matrix to check if values were correctly added
	printf("Matrix A:\n");
	printMatrix(A);
	printf("\n Matrix B:\n");
	printMatrix(B);

	// create threads to evaluate Matrix C where C = A X B
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			tp[i][j].i = i;
			tp[i][j].j = j;
			s = pthread_create(&tid[i][j], NULL, &multValue, &tp[i][j]);
			if(s!=0) handle_error("pthread_create for multValue failure");
		}
	}

	for(i = 0; i < N; i++){
		for(j = 0; j<N; j++){
			s = pthread_join(tid[i][j], NULL);
			if(s!=0) handle_error("pthread_join for multValue failure"); 
		}
	}
	printf("\n Matrix C:\n");
	printMatrix(C);
	
	// Create N threads to find the MAX_ROW_SUM
	for(i = 0; i <N; i++){
		s = pthread_create(&tid[i][0], NULL, &maxRowSum, (void *)&i);
		if (s!=0) handle_error("ptrhead_create for maxRowSum failure");
	}
	
	for(i = 0; i < N; i++){
		s = pthread_join(tid[i][0], NULL);
		if(s!=0) handle_error("pthread_join for maxRowSum failture");
	}
	printf("\nMAX ROW SUM = %d\n", MAX_ROW_SUM);
	
	pthread_mutex_destroy(&mutex);
	exit(EXIT_SUCCESS);

}

void *addValue(void * arg){
	struct tuple *tp = arg;
	int i, j;
	
	i = tp->i;
	j = tp->j;

	//make sure A and B are using different value of seeds
	A[i][j] = rand_r(&seeds[i][j])%(5*N*N);	
	B[i][j] = rand_r(&seeds[j][i])%(5*N*N);

	return NULL;
}


void *multValue(void * arg){
	struct tuple *tp = arg;
	int i,j, k;
	
	i = tp->i;
	j = tp->j;
	
	C[i][j] = 0;
	for(k =0; k < N; k++){
		C[i][j] += A[i][k]*B[k][j];
	}
	return NULL;	
}

/*computes the sum of the given row and update the MAX_ROW_SUM value*/
void *maxRowSum(void *arg){
	int i, sec,  sum = 0;
	int row = *(int *) arg;
	
	for(i=0; i < N; i++){
		sum += C[row][i]; 
	}

	sec= rand_r(&seeds[row][0])%MAX_SLEEP;
	sleep(sec);
		
	pthread_mutex_lock(&mutex);
	if(sum > MAX_ROW_SUM) MAX_ROW_SUM = sum;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

/****************************** UTILITY FUNCTIONS *****************************/

/*print error message and exit the process */
void handle_error(char* msg){
	perror(msg);
	exit(EXIT_FAILURE);
}

/*prints a N by N matrix*/
void printMatrix(int M[N][N]){
	int i, j;
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			printf("[%d][%d]:%d\t", i, j, M[i][j]);
		}
		printf("\n");
	}
}
