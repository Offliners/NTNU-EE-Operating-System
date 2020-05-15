#include <stdio.h>
#include <omp.h>
#define N 4

int main(){
	int i, j;

	#pragma omp parallel for private(i, j)
	for(i=0; i<N; i++){
		for(j=0; j<N; j++){
			printf("i:%d, j:%d\n", i, j);
		}
	}


	return 0;
}
