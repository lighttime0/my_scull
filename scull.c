#include <linux/init.h>	
#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/kernel.h>		/* printk() */
#include <linux/types.h>		/* Data types */
#include <linux/kdev_t.h>		/* MKDEV */
#include <linux/fs.h>			/* Register dev */

int scull_major = 0;
int scull_minor = 0;
int scull_nr_devs = 1;			/* Number of vare scull devices */

void scull_clean_module(void)
{
	dev_t dev = MKDEV(scull_major, scull_minor);

	if (scull_major)		//If scull_major doesn't equal 0, means scull allocated successfully
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
