# Practice 6
### Vector addition using pthread
Please evaluate the performance and try to find the problem
```C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100


int A[N];
int B[N];
int C[N];
int goldenC[N];
struct v {
   int pos; 
};

void *runner(void *param); /* the thread */

int main(int argc, char *argv[]) {
	int i, j, k;
	pthread_t tid[N];       //Thread ID
	pthread_attr_t attr[N]; //Set of thread attributes
	struct timespec t_start, t_end;
	double elapsedTime;
	
	for(i = 0; i < N; i++) {
	    A[i] = rand()%100;
		B[i] = rand()%100;
		
	}	
	
	// start time
	clock_gettime( CLOCK_REALTIME, &t_start);  	
	
	for(i = 0; i < N; i++) {
		    //Assign a row and column for each thread
			struct v *data = (struct v *) malloc(sizeof(struct v));
			data->pos = i;

			pthread_attr_init(&attr[i]);
			//Create the thread
			pthread_create(&tid[i],&attr[i],runner,data);

	}
	
	for(i = 0; i < N; i++) {
		pthread_join(tid[i], NULL);
	}	
	// stop time
	clock_gettime( CLOCK_REALTIME, &t_end);

	// compute and print the elapsed time in millisec
	elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
	elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
	printf("Parallel elapsedTime: %lf ms\n", elapsedTime);	
	//Print out the resulting matrix
	
	
	// start time
	clock_gettime( CLOCK_REALTIME, &t_start);  
	for(i = 0; i < N; i++) {
		goldenC[i]=A[i] * B[i];
	}
	// stop time
	clock_gettime( CLOCK_REALTIME, &t_end);

	// compute and print the elapsed time in millisec
	elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
	elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
	printf("Sequential elapsedTime: %lf ms\n", elapsedTime);	
	
	int pass = 1;
	for(i = 0; i < N; i++) {
		if(goldenC[i]!=C[i]){
				pass = 0;
		}
	}	
	if(pass==1)
		printf("Test pass!\n");
	else
		printf("Test fail!\n");
	
	return 0;
}

																					//The thread will begin control in this function
void *runner(void *param) {
	struct v *data = param; 
	int n; 
	C[data->pos] = A[data->pos] * B[data->pos];
	
	pthread_exit(0);
}
```
[code](vectorAdd.c)
