# Homework 2
### Endian Checker
Write a program on your virtual machine to test if your machine is little endian or big endian.

#### Solution
```C
#include<stdio.h>

int main()
{
        int test = 0x12345678;
        char *address = (char*)&test;

        if(*address == 0x78)
                printf("Little Endian\n");
        else if(*address == 0x12)
                printf("Big Endian\n");
        else
                printf("Unknown Endian\n");

        return 0;
}
```

[code](endian.c)

### Copy file 
Write a program to copy the contents of one file to a destination file. 

#### Solution
```C
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
```

[code](copy.c)
