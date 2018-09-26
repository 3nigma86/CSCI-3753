#include <linux/kernel.h>
#include <linux/linkage.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
asmlinkage long sys_cs3753_add(long number1,long number2,long *result)
{
long *sum;
sum = kmalloc(sizeof(long), GFP_KERNEL);
*sum = number1 + number2;
printk(KERN_ALERT "Number one = %ld \n",number1);
printk(KERN_ALERT "Number two = %ld \n",number2);
copy_to_user(result, sum, sizeof(long));
//printk(KERN_ALERT "The result = %ld \n",*result);
return 0;
}
