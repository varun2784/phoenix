#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>

static int ofd_init(void) {
	printk(KERN_INFO "ofd registered");
	return 0;
}

static void ofd_exit(void) {
	printk(KERN_INFO "vsb: ofd exit");
}

module_init(ofd_init);
module_exit(ofd_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("VSB");
MODULE_DESCRIPTION("test");
