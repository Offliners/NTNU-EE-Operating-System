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
