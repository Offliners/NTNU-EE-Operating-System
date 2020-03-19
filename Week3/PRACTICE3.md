# Practice 3
### Linux kernel modules
建立Makefile，並寫簡易C程式來載入與刪除模組
```makefile
obj-m += simple.o
all:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

[code](Makefile)

```C
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>

int simple_init(void)
{
        printk(KERN_INFO "Loading Module\n");
        return 0;
}

void simple_exit(void)
{
        printk(KERN_INFO "Removing Module\n");
}

module_init(simple_init);
module_exit(simple_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

```

[code](simple.c)
