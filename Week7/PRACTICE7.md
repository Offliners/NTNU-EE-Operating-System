# Practice 7 
### Matrix multiplication using pthread
```C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100
#define NUM_THREADS 10000

int A [N][N];
int B [N][N];
int C [N][N];
int goldenC [N][N];
struct v {
   int i; /* row */
   int j; /* column */
};

void *runner(void *param); /* the thread */

int main(int argc, char *argv[]) {
	int i, j, k;
	pthread_t tid[N][N];       //Thread ID
	pthread_attr_t attr[N][N]; //Set of thread attributes
	struct timespec t_start, t_end;
	double elapsedTime;
	
	for(i = 0; i < N; i++) {
	    for(j = 0; j < N; j++) {
			A[i][j] = rand()%100;
			B[i][j] = rand()%100;
		}
	}	
	
	// start time
	clock_gettime( CLOCK_REALTIME, &t_start);  	
	
	for(i = 0; i < N; i++) {
	    for(j = 0; j < N; j++) {
		    //Assign a row and column for each thread
			struct v *data = (struct v *) malloc(sizeof(struct v));
			data->i = i;
			data->j = j;
			/* Now create the thread passing it data as a parameter */
			//Get the default attributes
			pthread_attr_init(&attr[i][j]);
			//Create the thread
			pthread_create(&tid[i][j],&attr[i][j],runner,data);

		}
	}
	for(i = 0; i < N; i++) {
	    for(j = 0; j < N; j++) {
			pthread_join(tid[i][j], NULL);
		}
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
		for(j = 0; j < N; j++) {
			for(k=0; k<N; k++){
				goldenC[i][j]+=A[i][k] * B[k][j];
			}
		}
	}
	// stop time
	clock_gettime( CLOCK_REALTIME, &t_end);

	// compute and print the elapsed time in millisec
	elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
	elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
	printf("Sequential elapsedTime: %lf ms\n", elapsedTime);	
	
	int pass = 1;
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			if(goldenC[i][j]!=C[i][j]){
				pass = 0;
			}
		}
	}	
	if(pass==1)
		printf("Test pass!\n");
	
	return 0;
}

//The thread will begin control in this function
void *runner(void *param) {
	struct v *data = param; 
	int k, sum = 0; 
	for(k = 0; k< N; k++){
		sum += A[data->i][k] * B[k][data->j];
	}
	C[data->i][data->j] = sum;
	pthread_exit(0);
}
```
[code](matrixMul_pthread.c)

### Openmp example
```C
#include <stdio.h>

#include <stdlib.h>

#include <omp.h>

int main(){

   printf("The main thread\n");

   #pragma omp parallel

   {

   printf("The number of threads is %d\n", omp_get_num_threads());

   printf("The thread %d\n", omp_get_thread_num());

   }

   return 0;
}
```
[code](openmp_example.c)

### Vector addition using openMP
```C
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define size 100000000

int  main(){

   int i, pass = 1;
   int *A, *B, *C, *D;
   struct timespec t_start, t_end;
   double elapsedTime;
   A = (int*)malloc(size * sizeof(int));
   B = (int*)malloc(size * sizeof(int));
   C = (int*)malloc(size * sizeof(int));
   D = (int*)malloc(size * sizeof(int));

   for(i=0;i<size;i++){
      A[i] = rand()% 256;
      B[i] = rand()% 256;
   }

   // start time

   clock_gettime( CLOCK_REALTIME, &t_start);

   for(i=0;i<size; i++){
   	C[i] = A[i] + B[i];
   }

   // stop time

   clock_gettime( CLOCK_REALTIME, &t_end);

   // compute and print the elapsed time in millisec

   elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
   elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
   printf("elapsedTime: %lf ms\n", elapsedTime);


   // start time

   clock_gettime( CLOCK_REALTIME, &t_start);
   #pragma omp parallel for
   for(i=0;i<size; i++){
	D[i] = A[i] + B[i];
   }
   // stop time
    clock_gettime( CLOCK_REALTIME, &t_end);

   // compute and print the elapsed time in millisec
   elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
   elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
   printf("elapsedTime: %lf ms\n", elapsedTime);
   for(i=0; i<size; i++){
   	if(C[i]!=D[i]){
	   printf("error at A[%d]=%d, B[%d]=%d\n", i, A[i], i, B[i]);
	   pass = 0;
           break;
	}
   }
   if(pass==1)
	printf("Pass!\n");

   return 0;
}
```
[code](vectorAddition_openmp.c)

### Find and fix the bug
```C
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define N 10000000
int main(){
   struct timespec t_start, t_end;
   double elapsedTime;
   int i;
   int temp;
   int *A, *B, *AA, *BB;
   A = (int*)malloc(N*sizeof(int));
   B = (int*)malloc(N*sizeof(int));
   AA = (int*)malloc(N*sizeof(int));
   BB = (int*)malloc(N*sizeof(int));   
   for(i=0; i<N; i++){
	   A[i] = rand() % 256;
	   B[i] = rand() % 256;
	   AA[i] = A[i];
	   BB[i] = B[i];
   }
   // start time
   clock_gettime( CLOCK_REALTIME, &t_start);
   for(i=0; i<N; i++){
	   temp = A[i];
	   A[i] = B[i];
	   B[i] = temp;
   }
   // stop time
   clock_gettime( CLOCK_REALTIME, &t_end);

   // compute and print the elapsed time in millisec
   elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
   elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
   printf("sequential elapsedTime: %lf ms\n", elapsedTime);
	
   // start time
   clock_gettime( CLOCK_REALTIME, &t_start);
   #pragma omp parallel for 
   for(i=0; i<N; i++){
	   	temp = AA[i];
		AA[i] = BB[i];
		BB[i] = temp;
   }
   // stop time
   clock_gettime( CLOCK_REALTIME, &t_end);

   // compute and print the elapsed time in millisec
   elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
   elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
   printf("Parallel elapsedTime: %lf ms\n", elapsedTime);	

   for(i=0; i<N; i++){
	   if(A[i] != AA[i] || B[i]!=BB[i])
		   break;
   }
   if(i==N)
		printf("Test pass!!!\n");
	else	
		printf("Test failure\n");
	return 0;
}
```
[code](openmp_bug.c)

solution : #pragma omp parallel for private(temp)
