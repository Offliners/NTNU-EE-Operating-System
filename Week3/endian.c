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
