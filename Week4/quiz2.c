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
