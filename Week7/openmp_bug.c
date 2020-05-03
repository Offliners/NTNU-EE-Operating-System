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
