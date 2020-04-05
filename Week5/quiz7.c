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
