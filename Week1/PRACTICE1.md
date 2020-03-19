# Practice 1
### Hello VIM
使用VIM寫一個簡單的C程式，並使用gcc編譯並執行
```c
#include<stdio.h>

int main()
{
        int a, b;
        int c;

        printf("Please input a, b : ");
        scanf("%d %d", &a, &b);
        c = a + b;
        printf("a + b = %d\n", c);

        return 0;
}
```
[code](hello.c)
