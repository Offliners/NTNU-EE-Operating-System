#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#define N 100

int main(){
	struct timespec t_start, t_end;
	double elapsedTime;
	int i, j, k;
	int temp;
	int a[N][N], b[N][N], c[N][N], cc[N][N];
   
	for( i=0; i<N; i++ ){
		for( j=0; j<N; j++ ) {
			a[i][j] = rand()%10;
			b[i][j] = rand()%10;
		}
	}
	
	// start time
	clock_gettime( CLOCK_REALTIME, &t_start); 

   	//transpose of b	
	for( i=0; i<N; i++ ){
		for( j=i+1; j<N; j++ ) {
			temp = b[j][i];
			b[j][i] = b[i][j];
			b[i][j] = temp;
		}
	}

	for( i=0; i<N; i++ ){
		for( j=0; j<N; j++ ) {
			cc[i][j] = 0;
			for( k=0; k<N; k++ ){
				cc[i][j] += a[i][k]*b[j][k];
			}
		}
	}
	// stop time
	clock_gettime( CLOCK_REALTIME, &t_end);

	// compute and print the elapsed time in millisec
	elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
	elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
	printf("Sequential elapsedTime: %lf ms\n", elapsedTime);
  
 
   return 0;
}
