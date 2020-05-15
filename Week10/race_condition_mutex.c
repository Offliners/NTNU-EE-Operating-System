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
