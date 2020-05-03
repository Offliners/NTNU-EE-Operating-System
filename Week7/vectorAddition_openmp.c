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
