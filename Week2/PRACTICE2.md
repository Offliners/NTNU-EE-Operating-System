# Practice 2
### write a command with C
使用C語言寫一個命令，能進行兩數字相加並顯示結果。
```C
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[])
{
        if(argc != 3)
        {
                printf("Please use command %s [integer1] [integer2]\n", argv[0]);
                exit(1);
        }
        printf("%d\n", atoi(argv[1]) + atoi(argv[2]));
        return 0;
}
```

[code](add.c)
