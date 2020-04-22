# Homework 4: vector addition using 4 pthreads
從上一個例子裡面我們發現使用太多的pthreads是不行的，最佳的方式應該是有多少核心就使用多少個thread

請更改之前的範例，改成allocate 4 or 8 threads，並evaluate performance，重點是multiple thread的版本必須贏過single thread的版本！

```C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100000000
#define MAX_THREAD 4

int A[N];
int B[N];
int C[N];

int goldenC[N];

void *runner(void *arg);

int main(int argc, char *argv[]) {
        int i, j, k;
        pthread_t tid[MAX_THREAD];
        pthread_attr_t attr[MAX_THREAD];
        struct timespec t_start, t_end;
        double elapsedTime;

        for(i = 0; i < N; i++) {
                A[i] = rand()%100;
                B[i] = rand()%100;
        }

        clock_gettime( CLOCK_REALTIME, &t_start);

        for(i = 0; i < MAX_THREAD; i++) {
                int *arg = malloc(sizeof(int));
                *arg = i;
                pthread_attr_init(&attr[i]);
                pthread_create(&tid[i], &attr[i], runner, (void*)arg);
        }

        for(i = 0; i < MAX_THREAD; i++)
                pthread_join(tid[i], NULL);

        clock_gettime( CLOCK_REALTIME, &t_end);

        elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
        elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
        printf("Parallel elapsedTime: %lf ms\n", elapsedTime);

        clock_gettime( CLOCK_REALTIME, &t_start);
        for(i = 0; i < N; i++)
                goldenC[i] = A[i] * B[i];

        clock_gettime( CLOCK_REALTIME, &t_end);

        elapsedTime = (t_end.tv_sec - t_start.tv_sec) * 1000.0;
        elapsedTime += (t_end.tv_nsec - t_start.tv_nsec) / 1000000.0;
        printf("Sequential elapsedTime: %lf ms\n", elapsedTime);     

        int pass = 1;
        for(i = 0; i < N; i++)
                if(goldenC[i]!=C[i])
                        pass = 0;
                        
        if(pass==1)
                printf("Test pass!\n");
        else    
                printf("Test fail!\n");
                
        return 0;
}                

void *runner(void *arg) {
        int n = *((int*)arg);

        for(int i = (N / MAX_THREAD) * n; i < (N / MAX_THREAD) * n + N/MAX_THREAD; i++)
                C[i] = A[i] * B[i];

        pthread_exit(0);
}
```
[code](vectorAddpthread.c)

### Result
* Hardware : `Intel(R) Core(TM) i5-7300HQ`
![4pthread](https://github.com/Offliners/NTNU-EE-Operating-System/blob/master/Week6/4pthread.png)

![8pthread](https://github.com/Offliners/NTNU-EE-Operating-System/blob/master/Week6/8pthread.png)
