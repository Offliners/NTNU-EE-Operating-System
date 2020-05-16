# Homework 1
### Convert big decimal number to binary
寫一個command line輸入一個十進制數，可以輸出其二進制數值

輸入的十進制數可以超出long long的最大值

#### Solution
```C
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAXSIZE 100000

int main(int argc, char* argv[])
{
        char carry[MAXSIZE];
        int bin[MAXSIZE/10];
        int i, j = 0, k, n, len, sum = 1;

        if(argc != 2)
        {
                printf("Please use command %s [number]", argv[0]);
                exit(1);
        }

        len = strlen(argv[1]);
        carry[0] = '0';
        for(i = 0; i < len; i++)
                bin[i] = argv[1][i] - '0';
        while(sum)
        {
                sum = 0;
                for(i = 0; i < len; i++)
                {
                        k = bin[i] / 2;
                        sum += k;
                        if(i == len - 1)
                                carry[j++] = bin[i] % 2 + '0';
                        else
                                bin[i+1] += bin[i] % 2 * 10;
                        bin[i] = k;
                }
        }
        for(j = j - 1; j >= 0; j--)
                printf("%c",carry[j]);
        printf("\n");
        return 0;
}
```

[code](int2bin.c)
