#include <linux/init.h>
#include <linux/module.h>
MODULE_LICENSE("Dual BSD/GPL");

static int hello_init(void)
{
	printk(KERN_ALERT "Hello, world\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
/**
 * sudo insmod ./hello.ko
 * sudo rmmod hello
 *
 * #we can see log in /var/log/syslog
 * #we can list mod by command:lsmod
 */
