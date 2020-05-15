# Practice 8
### Complete the entire openMP #pragma and evaluate the performance
```C
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define N 1000
int main(){
   struct timespec t_start, t_end;
   double elapsedTime;
   int i;
   int *A, *B, *C, *CC;
   int sum = 0, parallelSum=0;
   A = (int*)malloc(N*sizeof(int));
   B = (int*)malloc(N*sizeof(int));
   C = (int*)malloc(N*sizeof(int));
   CC = (int*)malloc(N*sizeof(int));   
   for(i=0; i<N; i++){
	   A[i] = rand() % 256;
	   B[i] = rand() % 256;
   }
   // start time
   clock_gettime( CLOCK_REALTIME, &t_start);
   for(i=0; i<N; i++){
	   C[i] = A[i] + B[i];
	   sum += C[i];
   }
   // stop time
   clock_gettime( CLOCK_REALTIME, &t_end);

   // compute and print the elapsed time in millisec
   elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
   elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
   printf("sequential elapsedTime: %lf ms\n", elapsedTime);
	
   // start time
   clock_gettime( CLOCK_REALTIME, &t_start);
   #pragma omp ???
   {
   #pragma omp ???
   for(i=0; i<N; i++){
	   CC[i] = A[i] + B[i];
   }
   #pragma omp ????
   for(i=0; i<N; i++){
	   parallelSum += CC[i];
   }
   }
   // stop time
   clock_gettime( CLOCK_REALTIME, &t_end);

   // compute and print the elapsed time in millisec
   elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
   elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
   printf("Parallel elapsedTime: %lf ms\n", elapsedTime);	

   for(i=0; i<N; i++){
	   if(CC[i] != C[i])
		   break;
   }
   if(parallelSum != sum){
	   printf("parallelSum != sum\n");
   }
   if(i==N && parallelSum == sum)
		printf("Test pass!!!\n");

	return 0;
}
```
[code](openMP1.c)
