# Homework 6 : Producer Consumer Problem
Please solve Producer Consumer Problem
```C
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<semaphore.h>

typedef int buffer_item;
#define BUFFER_SIZE 5

buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t full, empty;
int count, in, out;

int insert_item(buffer_item item);
int remove_item(buffer_item *item);
void *consumer(void *param);
void *producer(void *param);

int main(int argc, char *argv[])
{
        if(argc != 4)
        {
                printf("Please use command %s [number] [number] [number]\n", argv[0]);
                exit(1);
        }

        const long int stime = strtol(argv[1], NULL, 0);
        const long int num_producer = strtol(argv[2], NULL, 0);
        const long int num_consumer = strtol(argv[3], NULL, 0);

        srand(time(NULL));
        pthread_mutex_init(&mutex, NULL);
        sem_init(&empty, 0, BUFFER_SIZE);
        sem_init(&full, 0, 0);
        count = in = out = 0;

        pthread_t producers[num_producer];
        pthread_t consumers[num_consumer];
        for(int i = 0; i < num_producer; i++) 
                pthread_create(&producers[i], NULL, producer, NULL);

        for(int i = 0; i < num_consumer; i++) 
                pthread_create(&consumers[i], NULL, consumer, NULL);

        sleep(stime);

        return 0;
}

int insert_item(buffer_item item)
{
        int flag;
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        if(count != BUFFER_SIZE)
        {
                buffer[in] = item;
                in = (in + 1) % BUFFER_SIZE;
                count++;
                flag = 0;
        }
        else
                flag = -1;

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        return flag;
}

int remove_item(buffer_item *item)
{
        int flag;
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if(count != 0){
                *item = buffer[out];
                out = (out + 1) % BUFFER_SIZE;
                count--;
                flag = 0;
        }
        else
                flag = -1;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        return flag;
}

void *producer(void *param)
{
        buffer_item item;

        while(1)
        {
                sleep(rand() % 5 + 1);

                item = rand();
                if(insert_item(item))
                        printf("Error\n");
                else
                        printf("Producer produced %d\n", item);
        }
}

void *consumer(void *param)
{
        buffer_item item;

        while(1)
        {
                sleep(rand() % 5 + 1);

                if(remove_item(&item))
                        printf("Error\n");
                else
                        printf("Consumer consumed %d\n", item);
        }
}

```
[code](consumer-producer.c)
