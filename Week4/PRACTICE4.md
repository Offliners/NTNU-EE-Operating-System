# Practice 4
### Example 1
比較父程序與子程序
```C
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main()
{
        pid_t child_pid = fork();

        if(child_pid > 0)
                sleep(50);
        else
                exit(0);

        return 0;
}
```
[code](example1.c)

### 孤兒行程 Orphan Process
使用C語言建立簡單的孤兒行程
```C
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>

int main()
{
        int pid = fork();

        if(pid > 0)
                printf("In parent process\n");
        else if(pid == 0)
        {
                sleep(30);
                printf("In child process\n");
        }

        return 0;
}
```
[code](orphan.c)

### Simple Fork
建立簡易Fork程式，觀察父程序與子程序
```C
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>


int main()
{
        pid_t pid;
        int status;

        pid = fork();
        if(pid < 0)
        {
                fprintf(stderr, "Fork failed!\n");
                exit(-1);
        }
        else if(pid == 0)
        {
                printf("PID : %d\n", pid);
                execlp("/bin/ls", "ls", NULL);
        }
        else
        {
                printf("Child process id : %d\n", pid);
                pid = wait(&status);
                printf("PID : %d, status : %d\n", pid, status);
                printf("Child Complete\n");
                exit(0);
        }

        return 0;
}
```
[code](fork.c)
