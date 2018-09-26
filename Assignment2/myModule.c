#include<linux/fs.h>
#include<linux/uaccess.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/slab.h> 


loff_t simple_llseek (struct file *filp, loff_t offset, int whence);
ssize_t simple_read (struct file *filp, char __user *, size_t size, loff_t *f_pos);
ssize_t simple_write (struct file *filp, const char __user *, size_t size, loff_t *offset);
int simple_open (struct inode *inode, struct file *filp);
int simple_release (struct inode *inode, struct file *filp);

#define MAX_SIZE 1024
static char charbuffer[MAX_SIZE];

struct file_operations simple_file_ops = {
	read: simple_read,
	write: simple_write,
	open: simple_open,
	release: simple_release,
	llseek: simple_llseek,
};


int simple_open(struct inode *inode, struct file *filp){
	static int count=0;
	count++;
	printk("This has been opened %d times\n", count);
	return 0;
}

int simple_release(struct inode *inode, struct file *filp){
	static int countClose=0;
	countClose++;
	printk("This has been closed %d times\n", countClose);
	return 0;
}

ssize_t simple_read(struct file *filp, char __user *buff, size_t size, loff_t *f_pos){
	int buffSize = strlen(charbuffer);
	if (buff==0){
		printk(KERN_ALERT "Buffer empty");
		return 0;
	}
	printk(KERN_ALERT "Reading from device\n");
	
	copy_to_user(buff, charbuffer, size);
	printk(KERN_ALERT "Device has read %d bytes\n", buffSize);
	return 0;
}

ssize_t simple_write (struct file *filp, const char __user *buff, size_t size, loff_t *pos){
	int buffSize = strlen(charbuffer);
	if (size ==0){
		return 0;
	}

	printk(KERN_ALERT "Writing to device\n");
	copy_from_user(charbuffer + *pos, buff, size);
	printk(KERN_ALERT "Device has written %zu bytes", buffSize-*pos);
	return size;
}

loff_t simple_llseek (struct file *filp, loff_t offset, int whence){

	loff_t newPosition =0;
	switch(whence){
		case 0:{
			newPosition = offset;
			break;
		}
		case 1:{
			newPosition = offset + filp->f_pos;
			break;
		}
		case 2:{
			newPosition = MAX_SIZE - offset;
			break;
		}
	}

	if(newPosition > MAX_SIZE){
		newPosition = MAX_SIZE;
		printk(KERN_ALERT "Position is not in the file\n");
	}
	if(newPosition < 0){
		newPosition = 0;
		printk(KERN_ALERT "Position is not in the file\n");
	}
	filp->f_pos = newPosition;
			
	
	return newPosition;
    	
}

static int simple_init(void)
{
	register_chrdev(300, "simple_character_device", &simple_file_ops);
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	return 0;
}

void simple_exit(void)
{
	unregister_chrdev(300, "simple_character_device");
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	
}

module_init(simple_init);
module_exit(simple_exit);
