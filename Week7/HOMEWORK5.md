# Homework 5: accelerate matrix multiplication C=A x B using pthread and openMP and compare their results
1.Please use openMP to accelerate matrix multiplication C=A x B
```C
for( i=0; i<N; i++ )

   for( j=0; j<N; j++ ) {

      c[i][j] = 0;

      for( k=0; k<N; k++ )

         c[i][j] += a[i][k]*b[k][j];
}
```
2. Please submit the report and the original code.
```C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include<omp.h>

#define N 4
#define MAX_THREADS 4

int A [N][N];
int B [N][N];
int C [N][N];
int T [N][N];
int O [N][N];
int goldenC [N][N];

void *runner(void *arg);

int main(int argc, char *argv[]) {
        int i, j, k;
        pthread_t tid[MAX_THREADS];
        pthread_attr_t attr[MAX_THREADS];
        struct timespec t_start, t_end;
        double elapsedTime;
        int temp;

        // Data generation
        for(i = 0; i < N; i++) {
            for(j = 0; j < N; j++) {
                        A[i][j] = rand()%100;
                        B[i][j] = rand()%100;
                }
        }

        // Parallel
        clock_gettime( CLOCK_REALTIME, &t_start);

        for(i = 0; i < MAX_THREADS; i++) {
                int *arg = malloc(sizeof(arg));
                *arg = i;
                pthread_attr_init(&attr[i]);
                pthread_create(&tid[i], &attr[i], runner, (void*)arg);
        }

        for(i = 0; i < MAX_THREADS; i++)
                pthread_join(tid[i], NULL);

        clock_gettime( CLOCK_REALTIME, &t_end);

        elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
        elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
        printf("Parallel elapsedTime (4-pthread): %lf ms\n", elapsedTime);

        // OpenMP
        clock_gettime( CLOCK_REALTIME, &t_start);

        #pragma omp parallel for private(i, j, k)
        for(i = 0; i < N; i++)
                for(j = 0; j < N; j++)
                        for(k = 0; k<N; k++)
                                O[i][j] += A[i][k] * B[k][j];

        clock_gettime( CLOCK_REALTIME, &t_end);

        elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
        elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
        printf("Parallel elapsedTime (OpenMP): %lf ms\n", elapsedTime);

        // Sequential 
        clock_gettime( CLOCK_REALTIME, &t_start);
        for(i = 0; i < N; i++) {
                for(j = 0; j < N; j++) {
                        for(k = 0; k<N; k++){
                                goldenC[i][j] += A[i][k] * B[k][j];
                        }
                }
        }

        clock_gettime( CLOCK_REALTIME, &t_end);

        elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
        elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
        printf("Sequential elapsedTime: %lf ms\n", elapsedTime);
        
        // Sequential with Transpose accleration
        clock_gettime( CLOCK_REALTIME, &t_start);

        for( i=0; i<N; i++ ){
                for( j=i+1; j<N; j++ ) {
                        temp = B[j][i];
                        B[j][i] = B[i][j];
                        B[i][j] = temp;
                }
        }

        for( i=0; i<N; i++ ){
                for( j=0; j<N; j++ ) {
                        T[i][j] = 0;
                        for( k=0; k<N; k++ )
                                T[i][j] += A[i][k]*B[j][k];
                }
        }

        clock_gettime( CLOCK_REALTIME, &t_end);

        elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
        elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
        printf("Sequential elapsedTime (Transpose accleration): %lf ms\n", elapsedTime);

        // Compare Result
        int pass = 1;
        for(i = 0; i < N; i++)
                for(j = 0; j < N; j++)
                        if((goldenC[i][j]^C[i][j]^T[i][j]^O[i][j]) != 0)
                                pass = 0;

        if(pass==1)
                printf("Test pass!\n");
        else
                printf("Test fail!\n");

        // Print Data
        /*
        for(i = 0; i < N; i++)
                for(j = 0; j < N; j++)
                        printf("%d %d %d %d\n", C[i][j], O[i][j], goldenC[i][j], T[i][j]);
        */

        return 0;
}

void *runner(void *arg) {
        int n = *((int*)arg);

        for(int i = (N / MAX_THREADS) * n; i < (N / MAX_THREADS) * n + N/MAX_THREADS; i++)
                for(int j = 0; j < N; j++)
                        for(int k = 0; k < N; k++)
                                C[i][j] += A[i][k] * B[k][j];

        pthread_exit(0);
}
```
[code](compare.c)

![compare]()
