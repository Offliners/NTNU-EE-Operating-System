#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>

int main(int argc, char* argv[])
{
        int source, destination;
        char data[512];
        char select;
        size_t dataLen;

        if(argc != 3)
        {
                printf("Please use command %s [source file] [destination file]\n", argv[0]);
                exit(1);
        }

        source = open(argv[1], O_RDONLY);
        destination = open(argv[2], O_WRONLY | O_CREAT | O_EXCL);
        if(source == -1)
        {
                printf("Source file cannot be opened!\n");
                exit(1);
        }

        if(destination == -1)
        {
                printf("Distination file existed!\n");
                exit(1);
        }

        dataLen = read(source, data, 512);
        if(dataLen < 0)
        {
                printf("File is too big!\n");
                exit(1);
        }

        if(write(destination, data, dataLen) < 0)
        {
                printf("Copy failed!\n");
                exit(1);
        }
        close(destination);
        close(source);

        return 0;
}
