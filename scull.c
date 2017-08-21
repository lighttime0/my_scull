#include <linux/init.h>	
#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/kernel.h>		/* printk() */
#include <linux/types.h>		/* Data types, such as dev_t, ssize_t */
#include <linux/kdev_t.h>		/* MKDEV, MAJOR, MINOR */
#include <linux/fs.h>			/* Register dev */

#include <linux/cdev.h>
#include <linux/slab.h>			/* kmalloc(), kfree() */
#include <linux/errno.h>		/* error codes */
#include <asm/uaccess.h>		/* copy_*_user */

int scull_major = 0;
int scull_minor = 0;
int scull_nr_devs = 1;			/* Number of bare scull devices */

char *scull_data = NULL;


int scull_open(struct inode *cnt_inode, struct file *file_p)
{
	/*struct scull_dev *dev;

	dev = container_of(cnt_inode->i_cdev, struct scull_dev, cdev);	//container_of: <linux/kernel.h>
	file_p->private_data = dev;

	if ( (file_p->f_flags & O_ACCMODE) == O_WRONLY)
	{
		scull_trim(dev);
	}*/
	return 0;
}

int scull_release(struct inode *cnt_inode, struct file *file_p)
{
	return 0;
}

ssize_t scull_read(struct file *file_p, char __user *buff, size_t count, loff_t *f_ops)
{
	int device_data_length = strlen(scull_data);

	if (*f_ops >= device_data_length)
		return 0;

	if ((count + *f_ops) > device_data_length)
		count = device_data_length;

	/* function to copy kernel space buffer to user space */
	if (copy_to_user(buff, scull_data, count) != 0) {
		printk(KERN_ALERT "Kernel Space to User Space copy failure");
		return -EFAULT;
	}

	/* Increment the offset to the number of bytes read */
	*f_ops += count;

	/* Return the number of bytes copied to the user space */
	return count;	
}

ssize_t scull__write(struct file * file_p, const char *buff, size_t count, loff_t * f_ops)
{
	/* Free the previouosly stored data */
	if (scull_data)
		kfree(scull_data);

	/* Allocate new memory for holding the new data */
	scull_data = kmalloc((count * (sizeof(char *))), GFP_KERNEL);

	/* function to copy user space buffer to kernel space */
	if (copy_from_user(scull_data, buff, count) != 0) {
		printk(KERN_ALERT "User Space to Kernel Space copy failure");
		return -EFAULT;
	}

	/* Since our device is an array, we need to do this to terminate the string.
	 * Last character will get overwritten by a \0. Incase of an actual file,
	 * we will probably update the file size, IIUC.
	 */
	scull_data [count] = '\0';

	/* Return the number of bytes actually written. */
	return count;
}

//Connect system call(open,read,etc.) to functions above.
struct file_operations scull_fops = {
	.owner = THIS_MODULE,		//THIS_MODULE defined in <linux/module.h>
	.open = scull_open,
	.release = scull_release,
	.read = scull_read,
	.write = scull_write,
}

void scull_clean_module(void)
{
	dev_t dev = MKDEV(scull_major, scull_minor);

	if (scull_major)		//If scull_major doesn't equal 0, means scull alloc_chrdev_region successfully
	{	
		unregister_chrdev_region(dev, scull_nr_devs);
	}

	printk(KERN_INFO "Goodbye, LT! Your scull module has been removed!\n");
}

int scull_init_module(void)
{
	int ret = 0;
	dev_t dev = 0;

	printk(KERN_INFO "Hello, LT! Your scull module is initing...\n");
	ret = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs, "scull");
	scull_major = MAJOR(dev);
	if (ret < 0)
	{
		printk(KERN_WARNING "scull: can't get major %d device\n", scull_major);
		return ret;			//Allocation failed, do not need to clean up
	}

	printk(KERN_INFO "scull module load successfully, enjoy it!\n");

	return 0;
}

module_init(scull_init_module);
module_exit(scull_clean_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LiTong");
