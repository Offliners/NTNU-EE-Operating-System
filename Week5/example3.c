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
