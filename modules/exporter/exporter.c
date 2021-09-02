#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>


static int exp_scrap(int a) {
	printk("int a x 3 = %d", (int)(a * 3));
	return 0;
}

static int exp_init(void) {
	printk("exporter module loaded");
	printk("pointer to exportet function: %lx", (unsigned long)(exp_scrap));
	return 0;
}

static void exp_exit(void) {
	printk("exporter module removed");
}

EXPORT_SYMBOL(exp_scrap);
module_init(exp_init);
module_exit(exp_exit);
MODULE_INFO(livepatch, "Y");
