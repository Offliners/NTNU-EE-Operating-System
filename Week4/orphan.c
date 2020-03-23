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
