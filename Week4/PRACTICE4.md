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
#include<wait.h>

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

### Quiz1
What is the result?
```C
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int value = 5;

int main(){
        pid_t pid;
        pid = fork();
        
        if(pid == 0){
                value += 15;
        }
        else{
                wait(NULL);
                printf("Parent: value: %d\n", value);
        }
        return 0;
}
```
[code](quiz1.c)

### Quiz2
What are the values of lines A, B, C, and D?
```C
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(){
        pid_t pid, pid1;
        pid = fork();

        if(pid<0){
                fprintf(stderr, "Fork failed");
                return 1;
        }
        else if(pid == 0){
                pid1 = getpid();
                printf("child: pid = %d\n", pid);//Line A
                printf("child: pid1 = %d\n", pid1);//Line B
        }
        else{
                pid1 = getpid();
                printf("parent: pid = %d\n", pid);//Line C
                printf("parent: pid1 = %d\n", pid1);//Line D
        
        wait(NULL);
        }
}
```
[code](quiz2.c)

### Quiz3
How many processes are created?
```C
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(){
        int i;

        for(i=0; i<4; i++){
           fork();
        }

        return 0;
}
```
[code](quiz3.c)

### Quiz4
What is the result?
```C
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define SIZE 5

int nums[SIZE] = {0,1,2,3,4};

int main()
{
  int i;
  pid_t pid;
  pid = fork();

  if (pid == 0) {

        for (i = 0; i < SIZE; i++) {
                nums[i] *= -i;
                printf("CHILD %d\n",nums[i]); /* LINE X */
        }
  }

  else if (pid > 0) {
        wait(NULL);
        for (i = 0; i < SIZE; i++)
                printf("PARENT: %d\n",nums[i]); /* LINE Y */
  }

  return 0;
}
```
[code](quiz4.c)

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
