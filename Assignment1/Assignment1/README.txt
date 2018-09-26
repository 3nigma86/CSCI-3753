Student Information:
Name : Casey Durham
ID # : 105991899
Email : cadu3881@colorado.edu
Phone : 9727624417

FILES INCLUDED:

***cs3753_add.c*** - This file is a c program located within the kernel. 
 
This program takes two numbers as input and adds them within the kernel.

Located in /linux-hwe-4.13.0/arch/x86/kernel 




***Makefile*** - This is our build system.  Inside, the cs3573_add system call

is included.   

Located in /linux-hwe-4.13.0/arch/x86/kernel




***syscall_64.tbl***  - This file contains table of system calls within the 

kernel

Located in /linux-hwe-4.13.0/arch/x86/entry/syscalls




***syscalls.h*** - header file for the structure of the system calls variables.

Located in /linux-hwe-4.13.0/include/linux




***syslog*** - this is just a recent log from the linux kernel.  

Located in /var/log/syslog




***cs3753_addtest.c*** - c program code for testing the cs3753_add system call.

Located in /home/user/Documents


INSTRUCTIONS:
All files should be loaded into their respective directory. 

Run:  
sudo make -j2 CC="ccache gcc"
sudo make -j2 modules_install
sudo make -j2 install
sudo reboot 
while inside of linux-hwe-4.13.0 directory to compile the kernel.

While inside of 
/home/user/Documents, run gcc cs3753_addtest.c and then ./a.out.   



