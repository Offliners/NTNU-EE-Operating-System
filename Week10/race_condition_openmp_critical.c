#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define N 10000
int main(){
    int n[N];
    int i;
	int total = 0;
	
	for(i=0; i < N; i++){
		n[i] = i+1;  
	}
	
	#pragma omp parallel for
	for(i=0; i < N; i++){
		#pragma omp critical
		total += n[i];
	}
	
	printf("The total is %d\n", total);

	return 0;
}
