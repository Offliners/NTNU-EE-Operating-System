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
