#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>

int main () {
long number1 = 3;
long number2 = 3;
long *result = malloc(sizeof(long));

syscall(334, number1, number2, result);
printf("The result = %ld\n",*result);
return 0;
}