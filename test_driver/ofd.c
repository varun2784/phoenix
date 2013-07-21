#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

static dev_t first_dev;

static int __init ofd_init(void) {
	printk(KERN_INFO "vsb: ofd registered\n");
	if (alloc_chrdev_region(&first_dev, 0, 4, 
				¨vsb¨) < 0) {
		return -1;
	}
	printk(KERN_INFO "vsb: ofd <Major %d, Minor %d>\n", 
			MAJOR(first_dev), MINOR(first_dev));
	return 0;
}

static void __exit ofd_exit(void) {
	unregister_chrdev_region(first_dev, 3);
	printk(KERN_INFO "vsb: ofd unregistered\n");
}

module_init(ofd_init);
module_exit(ofd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VSB");
MODULE_DESCRIPTION("test");
