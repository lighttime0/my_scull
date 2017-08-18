#include <linux/init.h>	
#include <linux/module.h>
#include <linux/moduleparam.h>

void scull_clean_module(void)
{
	printk(KERN_INFO "Goodbye, LT! Your scull module has been removed!\n");
}

int scull_init_module(void)
{
	printk(KERN_INFO "Hello, LT! Your scull module is initing...\n");
	return 0;
}

module_init(scull_init_module);
module_exit(scull_clean_module);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LiTong");

