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
