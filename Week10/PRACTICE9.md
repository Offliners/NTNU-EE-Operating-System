# Practice 9
### race condition in pthread
```C
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
void *thread(void *param);
int count = 0;
#define N 10000
int main(int argc, char *argv[])
{
	int i;
	
	pthread_t tid[N];
	pthread_attr_t attr[N];

	//Create 10000 threads
	for(i = 0; i < N; i++)
	{
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i], &attr[i], thread, NULL);
	}
	
	for(i = 0; i < N; i++)
	{
 		pthread_join(tid[i], NULL);
	}
	printf("count is %d\n", count);

	return 0;
}

void *thread(void *param)
{
	count++;
}
```
[code](race_condition_pthread.c)

### Solve Race Condition with Mutex Lock
透過mutex lock解決race condition問題
```C
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *thread(void *param);
int count = 0;
pthread_mutex_t mutex;
#define N 10000
int main(int argc, char *argv[])
{
	int i;
	pthread_t tid[N];
	pthread_attr_t attr[N];
	pthread_mutex_init(&mutex, NULL);
	
	//Create N threads
	for(i = 0; i < N; i++)
	{
		pthread_attr_init(&attr[i]);
		pthread_create(&tid[i], &attr[i], thread, NULL);
	}
	
	for(i = 0; i < N; i++)
	{
		pthread_join(tid[i], NULL);
	}
	printf("count is %d\n", count);

	return 0;
}

void *thread(void *param)
{
	//acquire the mutex lock
	pthread_mutex_lock(&mutex);
	count++;
	//release the mutex lock
	pthread_mutex_unlock(&mutex);

}
```
[code](race_condition_mutex.c)

### Solve Race Condition with Semaphore
```C
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

void *thread(void *param);
int count = 0;
#define N 10000
sem_t sem;

int main(int argc, char *argv[])
{
	int i;
	pthread_t tid[N];
	pthread_attr_t attr[N];
	
	/*Create the semaphore and initialize it to 1*/
	sem_init(&sem, 0, 1);
	
	//Create N threads
	for(i = 0; i < N; i++)
	{
 		pthread_attr_init(&attr[i]);
 		pthread_create(&tid[i], &attr[i], thread, NULL);
	}
	for(i = 0; i < N; i++)
	{
 		pthread_join(tid[i], NULL);
	}
	printf("count is %d\n", count);

	return 0;
}

void *thread(void *param)
{
	//acquire the semaphore
	sem_wait(&sem);
	count++;
	//release the semaphore
	sem_post(&sem);
}
```
[code](race_condition_semaphore.c)

### race condition example in openmp
下面這個例子在執行時，答案會有不正確的時候，這個問題稱為race condition。原因是多個執行緒同時修改同一個變數。
```C
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
		total += n[i];
	}
	
	printf("The total is %d\n", total);

	return 0;
}
```
[code](race_condition_openmp.c)

### Solve Race Condition with OpenMP Atomic
```C
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
		#pragma omp atomic
		total += n[i];
	}
	
	printf("The total is %d\n", total);

	return 0;
}
```
[code](race_condition_openmp_atomic.c)

### Solve Race Condition with OpenMP Critical
```C
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
```
[code](race_condition_openmp_critical.c)

### Solve Race Condition with OpenMP Clause
```C
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
	
	#pragma omp parallel for reduction(+:total)
	for(i=0; i < N; i++){
		total += n[i];
	}
	
	printf("The total is %d\n", total);

	return 0;
}
```
[code](race_condition_openmp_clause.c)

### Task parallelism using pthread
Suppose your program is passed the integers: 90 81 78 95 79 72 85.

Your program creates three threads to calculate the average, the minimum, and the maximum in parallel.

The program report

The average value is 82

The minimum value is 72

The maximum value is 95 
```C
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

/* the list of integers */
int *list;

/* the threads will set these values */
double average;
int maximum;
int minimum;

void *calculate_average(void *param);
void *calculate_maximum(void *param);
void *calculate_minimum(void *param);

int main(int argc, char *argv[])
{
	int i;
	int num_of_args = argc-1;
	pthread_t tid_1;
	pthread_t tid_2;
	pthread_t tid_3;

	/* allocate memory to hold array of integers */
	list = malloc(sizeof(int)*num_of_args);

	for (i = 0; i < num_of_args; i++)
		list[i] = atoi(argv[i+1]);	

	/* create the threads */
	pthread_create(&tid_1, 0, calculate_average, &num_of_args);
	pthread_create(&tid_2, 0, calculate_maximum, &num_of_args);
	pthread_create(&tid_3, 0, calculate_minimum, &num_of_args);

	/* wait for the threads to exit */
	pthread_join(tid_1, NULL);
	pthread_join(tid_2, NULL);
	pthread_join(tid_3, NULL);

	printf("The average is %f\n", average);
	printf("The maximum is %d\n", maximum);
	printf("The minimum is %d\n", minimum);

	return 0;
}



void *calculate_average(void *param)
{
	int count = *(int *)param;
	int i, total = 0;

	printf("count = %d\n",count);
	for (i = 0; i < count; i++)
		printf("%d\n",list[i]);	

	for (i = 0; i < count; i++)
		total += list[i];

	average = (double) total / count;
	pthread_exit(0);
}



void *calculate_maximum(void *param)
{
	int count = *(int *)param;
	int i;

	maximum = list[0];

	for (i = 1; i < count; i++)
		if (list[i] > maximum)
			maximum = list[i];

	pthread_exit(0);
}

void *calculate_minimum(void *param)
{
	int count = *(int *)param;
	int i;

	minimum = list[0];

	for (i = 1; i < count; i++)
		if (list[i] < minimum)
			minimum = list[i];

	pthread_exit(0);
}
```
[code](task_parallel_pthread.c)
