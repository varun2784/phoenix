#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>

static int ofd_init(void) {
	printk(¨ofd registered\n¨);
	return 0;
}

static void ofd_exit(void) {
//	printk(¨vsb: ofd exit¨);
}

module_init(ofd_init);
module_exit(ofd_exit);

MODULE_LICENSE(¨GPL¨);
MODULE_AUTHOR();
MODULE_DESCRIPTION();
