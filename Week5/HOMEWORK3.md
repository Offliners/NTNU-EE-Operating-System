# Homework 3
1. 請用POSIX shared memory來實現producer process與comsumer process之間共享的circular buffer

2. producer每執行一次輸入一筆資料

3. comsumer每執行一次讀出一筆資料

4. 解決circular buffer會少用到一個空間的問題

#### Homework 3-1 : Initial share memory
```C
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>

# define N 8

int main(int argc, char *argv[])
{
        const int SIZE = N * sizeof(int);
        const char *shm_buffer_name = "shm_buffer";
        const char *shm_in_name = "shm_in";
        const char *shm_out_name = "shm_out";
        const char *shm_flag_name = "shm_flag";

        int shm_fd, shm_in, shm_out, shm_flag;
        static int *buffer, *in, *out, *flag;

        shm_fd = shm_open(shm_buffer_name, O_CREAT | O_RDWR, 0666);
        shm_in = shm_open(shm_in_name, O_CREAT | O_RDWR, 0666);
        shm_out = shm_open(shm_out_name, O_CREAT | O_RDWR, 0666);
        shm_flag = shm_open(shm_flag_name, O_CREAT | O_RDWR, 0666);

        ftruncate(shm_fd, SIZE);
        ftruncate(shm_in, sizeof(int));
        ftruncate(shm_out, sizeof(int));
        ftruncate(shm_flag, sizeof(int));

        buffer = (int*)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if(buffer == MAP_FAILED)
        {
                printf("Buffer Map failed!\n");
                return -1;
        }

        in = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_in, 0);
        if(in == MAP_FAILED)
        {
                printf("In Map Failed!\n");
                return -1;
        }

        out = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_out, 0);
        if(out == MAP_FAILED)
        {
                printf("Out Map Failed!\n");
                return -1;
        }

        flag = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_flag, 0);
	if(flag == MAP_FAILED)
        {
                printf("Out Map Failed!\n");
                return -1;
        }

        *in = 0;
        *out = 0;
        *flag = 0;

        printf("Initialize in : %d, out : %d, flag = %d\n", *in, *out, *flag);

        return 0;
}
```
[code](Initial.c)

#### Homework 3-2 : Producer
```C
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>

# define N 8

int main(int argc, char *argv[])
{
        const int SIZE = N * sizeof(int);
        const char *shm_buffer_name = "shm_buffer";
        const char *shm_in_name = "shm_in";
        const char *shm_out_name = "shm_out";
        const char *shm_flag_name = "shm_flag";

        if(argc != 2)
        {
                printf("Please use command : %s [integer]\n", argv[0]);
                exit(1);
        }

        int shm_fd, shm_in, shm_out, shm_flag;
        static int *buffer, *in, *out, *flag;

        shm_fd = shm_open(shm_buffer_name, O_CREAT | O_RDWR, 0666);
        if(shm_fd == -1)
        {
                printf("shared memory failed!\n");
                exit(-1);
        }

        shm_in = shm_open(shm_in_name, O_CREAT | O_RDWR, 0666);
        if(shm_in == -1)
        {
                printf("shared memory failed!\n");
                exit(-1);
        }

        shm_out = shm_open(shm_out_name, O_CREAT | O_RDWR, 0666);
        if(shm_out == -1)
        {
                printf("shared memory failed!\n");
                exit(-1);
        }

        shm_flag = shm_open(shm_flag_name, O_CREAT | O_RDWR, 0666);
        if(shm_flag == -1)
        {
                printf("shared memory failed!\n");
                exit(-1);
        }


        ftruncate(shm_fd, SIZE);
        ftruncate(shm_in, sizeof(int));
        ftruncate(shm_out, sizeof(int));
        ftruncate(shm_flag, sizeof(int));

        buffer = (int*)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if(buffer == MAP_FAILED)
        {
                printf("Map in failed!\n");
                return -1;
        }

        in = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_in, 0);
        if(in == MAP_FAILED)
        {
                printf("Map in failed!\n");
                return -1;
        }

        out = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_out, 0);
        if(out == MAP_FAILED)
        {
                printf("Map in failed!\n");
                return -1;
        }

        flag = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_flag, 0);
        if(flag == MAP_FAILED)
        {
                printf("Map in failed!\n");
                return -1;
        }

        if(*flag == 1)
        {
                printf("buffer full!\n");
                *in = N;
                printf("Next in : %d, out : %d, flag : %d\n", *in, *out, *flag);
                exit(1);
        }

        buffer[*in] = atoi(argv[1]);
        printf("Produce buffer[%d] : %d\n", *in, buffer[*in]);

        *in = (*in + 1) % N;

        if(*in == 0)
        {
                *flag += 1;
                *in = N;
        }

        printf("Next in : %d, out : %d, flag : %d\n", *in, *out, *flag);
        return 0;
}
```
[code](Producer.c)
#### Homework 3-3 : Consumer
```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define N 8

int main()
{
        const int SIZE = N * sizeof(int);
        const char *shm_buffer_name = "shm_buffer";
        const char *shm_in_name = "shm_in";
        const char *shm_out_name = "shm_out";
        const char *shm_flag_name = "shm_flag";

        int shm_fd, shm_in, shm_out, shm_flag;
        int *buffer, *in, *out, *flag;

        shm_fd = shm_open(shm_buffer_name, O_RDONLY, 0666);
        if (shm_fd == -1)
        {
                printf("shared memory failed\n");
                exit(-1);
        }

        shm_in = shm_open(shm_in_name, O_CREAT | O_RDWR, 0666);
        if (shm_in == -1)
        {
                printf("shared memory failed\n");
                exit(-1);
        }

        shm_out = shm_open(shm_out_name, O_CREAT | O_RDWR, 0666);
        if (shm_out == -1)
        {
                printf("shared memory failed\n");
                exit(-1);
        }

        shm_flag = shm_open(shm_flag_name, O_CREAT | O_RDWR, 0666);
        if (shm_flag == -1)
        {
                printf("shared memory failed\n");
                exit(-1);
        }

        ftruncate(shm_fd, SIZE);
        ftruncate(shm_in, sizeof(int));
        ftruncate(shm_out, sizeof(int));
        ftruncate(shm_flag, sizeof(int));

        buffer = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        if (buffer == MAP_FAILED)
        {
                printf("Map failed\n");
                exit(-1);
        }

        in = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_in, 0);
        if (in == MAP_FAILED)
        {
                printf("Map in failed\n");
                return -1;
        }

        out = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_out, 0);
        if (out == MAP_FAILED)
        {
                printf(" Map out failed\n");
                return -1;
        }

        flag = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_flag, 0);
        if (flag == MAP_FAILED)
        {
                printf(" Map out failed\n");
                return -1;
        }

        if(*flag == 0)
        {
                printf("buffer is empty!\n");
                printf("in : %d, next out : %d, flag : %d\n", *in, *out, *flag);
                exit(1);
        }

        printf("consumed buffer[%d]: %d\n", *out, buffer[*out]);
        *out = (*out + 1) % N;

        if(*in - *out == N)
                *flag -= 1;

        printf("in : %d, next out : %d, flag : %d\n", *in, *out, *flag);

        return 0;
}
```
[code](Consumer.c)
