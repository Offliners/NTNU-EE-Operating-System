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
