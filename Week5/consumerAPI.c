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
