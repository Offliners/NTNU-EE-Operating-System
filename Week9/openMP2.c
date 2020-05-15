#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define size 100000000

int  main(){

   int sum = 0;
   int i, *A;
   struct timespec t_start, t_end;
   double elapsedTime;
   A = (int*)malloc(size * sizeof(int));

   for(i=0;i<size;i++){
      A[i] = rand()% 2;
   }

   // start time

   clock_gettime( CLOCK_REALTIME, &t_start);

   for(i=0;i<size; i++)
   	sum = sum + A[i];


   // stop time

   clock_gettime( CLOCK_REALTIME, &t_end);

   // compute and print the elapsed time in millisec

   elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
   elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
   printf("elapsedTime: %lf ms\n", elapsedTime);
   printf("The sum is %d\n", sum);



   sum = 0;

   // start time

   clock_gettime( CLOCK_REALTIME, &t_start);
   #pragma omp parallel for num_threads(8) schedule(static) reduction(+:sum)
   for(i=0;i<size; i++)
	sum = sum + A[i];

   // stop time
    clock_gettime( CLOCK_REALTIME, &t_end);

   // compute and print the elapsed time in millisec
   elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
   elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
   printf("elapsedTime: %lf ms\n", elapsedTime);
   printf("The sum is %d\n", sum);
   return 0;

}
