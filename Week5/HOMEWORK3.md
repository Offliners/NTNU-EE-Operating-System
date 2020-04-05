# Homework 3
1. 請用POSIX shared memory來實現producer process與comsumer process之間共享的circular buffer

2. producer每執行一次輸入一筆資料

3. comsumer每執行一次讀出一筆資料

4. 解決circular buffer會少用到一個空間的問題

#### Example code: initial share memory
```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define N 8
int main(int argc, char* argv[]){

	const int SIZE = N*sizeof(int);
	const char *shm_buffer_name = "shm_buffer";
	const char *shm_in_name = "shm_in";
	const char *shm_out_name = "shm_out";

	int shm_fd, shm_in, shm_out;
	static int *buffer, *in, *out;

	/* create the shared memory segment */
	shm_fd = shm_open(shm_buffer_name, O_CREAT | O_RDWR, 0666);
	shm_in = shm_open(shm_in_name, O_CREAT | O_RDWR, 0666);
	shm_out = shm_open(shm_out_name, O_CREAT | O_RDWR, 0666);

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd, SIZE);
	ftruncate(shm_in, sizeof(int));
	ftruncate(shm_out, sizeof(int));

	/* now map the shared memory segment in the address space of the process */
	buffer = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (buffer == MAP_FAILED) {
		printf("Buffer Map failed\n");
		return -1;
	}

	in = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_in, 0);
	if (in == MAP_FAILED) {
		printf("In Map failed\n");
		return -1;
	}	
	out = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_out, 0);
	if (out == MAP_FAILED) {
		printf("Out Map failed\n");
		return -1;
	}	

	*in = 0;
	*out = 0;

	printf("Initialize in:%d, out:%d\n", *in, *out);	
	return 0;
}
```
[code](example1.c)


#### Example code: Producer using circular queue
```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#define N 8

int main(int argc, char* argv[]){

	const int SIZE = N*sizeof(int);
	const char *shm_buffer_name = "shm_buffer";
	const char *shm_in_name = "shm_in";
	const char *shm_out_name = "shm_out";

	if(argc!=2){
		printf("using command: %s [integer]", argv[0]);
		exit(1);
	}

	int shm_fd, shm_in, shm_out;
	static int *buffer, *in, *out;

	/* create the shared memory segment */
	shm_fd = shm_open(shm_buffer_name, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}
	
	shm_in = shm_open(shm_in_name, O_CREAT | O_RDWR, 0666);
	if (shm_in == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}
	
	shm_out = shm_open(shm_out_name, O_CREAT | O_RDWR, 0666);
	if (shm_out == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}
	
	/* configure the size of the shared memory segment */
	ftruncate(shm_fd, SIZE);
	ftruncate(shm_in, sizeof(int));
	ftruncate(shm_out, sizeof(int));

	/* now map the shared memory segment in the address space of the process */
	buffer = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (buffer == MAP_FAILED) {
		printf("Map buffer failed\n");
		return -1;
	}

	in = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_in, 0);
	if (in == MAP_FAILED) {
		printf("Map in failed\n");
		return -1;
	}
	out = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_out, 0);
	if (out == MAP_FAILED) {
		printf(" Map out failed\n");
		return -1;
	}

	while((*in+1)%N==*out){
		printf("buffer is full!\n");
		exit(1);	
	}

	buffer[*in] = atoi(argv[1]);
	printf("Produce buffer[%d]: %d\n", *in, buffer[*in]);
	*in = (*in + 1)%N;
	
	printf("Next in:%d, out:%d\n", *in, *out);	
	return 0;
}
```
[code](example2.c)

#### Example code: Consumer using circular queue
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
	const int SIZE = 8*sizeof(int);
	const char *shm_buffer_name = "shm_buffer";
	const char *shm_in_name = "shm_in";
	const char *shm_out_name = "shm_out";
	int shm_fd, shm_in, shm_out;
	int *buffer, *in, *out;


	int i;


	/* open the shared memory segment */
	shm_fd = shm_open(shm_buffer_name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}
	
	shm_in = shm_open(shm_in_name, O_CREAT | O_RDWR, 0666);
	if (shm_in == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}
	shm_out = shm_open(shm_out_name, O_CREAT | O_RDWR, 0666);
	if (shm_out == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* configure the size of the shared memory segment */
	ftruncate(shm_fd, SIZE);
	ftruncate(shm_in, sizeof(int));
	ftruncate(shm_out, sizeof(int));
	
	/* now map the shared memory segment in the address space of the process */
	buffer = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (buffer == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}

	in = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_in, 0);
	if (in == MAP_FAILED) {
		printf("Map in failed\n");
		return -1;
	}
	
	out = (int*)mmap(0, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_out, 0);
	if (out == MAP_FAILED) {
		printf(" Map out failed\n");
		return -1;
	}
		
	while(*in==*out){
		printf("buffer is empty!\n");
		exit(1);
	}
	printf("consumed buffer[%d]: %d\n", *out, buffer[*out]);
	*out = (*out + 1) % N;
    printf("in:%d, next out:%d\n", *in, *out);	

/*
	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}
*/
	return 0;
}
```
[code](example3.c)

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
[code](homework3_1.c)
