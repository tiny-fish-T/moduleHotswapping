#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

extern int exp_scrap(int a);

static int imp_init(void) {
	printk("importer loaded");
	printk("imported symbol value = %lx", (unsigned long)(exp_scrap));
	return 0;
}

static void imp_exit(void) {
	printk("importer removed");
}

module_init(imp_init);
module_exit(imp_exit);
MODULE_INFO(livepatch, "Y");
MODULE_INFO(kallsyms, "Y");
