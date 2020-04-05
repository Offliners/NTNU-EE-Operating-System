# Practice 5
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

### Producer process using POSIX shared-memory API
POSIX shared-memory is organized using memory-mapped files(記憶體映射檔案)

```C

```
