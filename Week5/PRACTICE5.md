# Practice 5
### Producer process using POSIX shared-memory API
POSIX shared-memory is organized using memory-mapped files(記憶體映射檔案)

`note` : 有使用shm_open時，gcc編譯需加`-lrt`
```C
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<unistd.h>
#include<string.h>

int main()
{
        const int SIZE = 4096;
        const char *name = "OS";
        const char *message0 = "Hello";
        const char *message1 = "World!";
        int shm_fd;
        void *ptr;

        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SIZE);
        ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

        sprintf(ptr, "%s", message0);
        ptr += strlen(message0);
        sprintf(ptr, "%s", message1);
        ptr += strlen(message1);

        return 0;
}
```
[code](producerAPI.c)

### Consumer Process
```C
#include<fcntl.h>
#include<sys/mman.h>
#include<stdio.h>

int main()
{
        const char *name = "OS";
        const int SIZE = 4096;

        int shm_fd;
        void *ptr;
        int i;

        shm_fd = shm_open(name, O_RDONLY, 0666);
        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        printf("%s", (char*)ptr);
        shm_unlink(name);

        return 0;
}
```
[code](consumerAPI.c)

### Quiz5
Try to using shared memory
```C
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int *glob_var;

int main(void)
{
    glob_var = mmap(NULL, sizeof *glob_var, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *glob_var = 1;
    if (fork() == 0) {
        *glob_var = 5;
        exit(EXIT_SUCCESS);
    } 
    else {
        wait(NULL);
        printf("%d\n", *glob_var);
        munmap(glob_var, sizeof *glob_var);
    }

    return 0;
}
```
[code](quiz5.c)


### Quiz 6
Using shared memory
```C
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/mman.h>
#include<wait.h>

#define SIZE 5

int main()
{
        int i;
        pid_t pid;
        int *nums;

        nums = mmap(NULL, SIZE * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        for(i = 0; i < SIZE; i++)
                nums[i] = i;

        pid = fork();
        if(pid == 0)
                for(i = 0; i < SIZE; i++)
                {
                        nums[i] *= -i;
                        printf("CHILD %d\n", nums[i]);
                }
        else if(pid > 0)
        {
                wait(NULL);
                for(i = 0; i < SIZE; i++)
                        printf("PARENT : %d\n", nums[i]);
        }

        return 0;
}

```
[code](quiz6.c)

### Quiz 7 
```C
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<wait.h>

#define SIZE 5

int main()
{
        int i;
        pid_t pid;
        int shm_fd;
        int *ptr;
        const char *name = "sharedMem";
        const int SHM_SIZE = SIZE * sizeof(int);

        shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, SHM_SIZE);
        ptr = (int*)mmap(0, SHM_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

        for(i = 0; i < SIZE; i++)
                ptr[i] = i;

        pid = fork();
        if(pid == 0)
                for(i = 0; i < SIZE; i++)
                {
                        ptr[i] *= -i;
                        printf("CHILD : %d\n", ptr[i]);
                }
        else if(pid > 0)
        {
                wait(NULL);
                for(i = 0; i < SIZE; i++)
                        printf("PARENT : %d\n", ptr[i]);
        }

        return 0;
}

```
[code](quiz7.c)

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
