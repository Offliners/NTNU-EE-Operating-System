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
