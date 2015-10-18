#include "version.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/utsname.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define MODULE_NAME		"LKM_version"
#define MAX_KMALLOC_SIZE	4096

static char* KernelVersion = NULL;
static char* CurrentVersion = NULL;
static char *Buffer = NULL;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Alexandre Frizac");
MODULE_DESCRIPTION("A simple loadable kernel module");


static long mylkm_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param);
static ssize_t mylkm_read (struct file *, char __user *, size_t, loff_t *);
static ssize_t mylkm_write (struct file *, const char __user *, size_t, loff_t *);

static struct file_operations fops = {
  .owner = THIS_MODULE,
  .read = mylkm_read,
  .write = mylkm_write,
  .unlocked_ioctl = mylkm_ioctl,
};

static struct miscdevice mdev = {
  .minor = MISC_DYNAMIC_MINOR,
  .name = "version",
  .fops = &fops,
};

int __init my_init_module(void)
{
  pr_info("%s:. Hello from module !\n", MODULE_NAME);

  KernelVersion = utsname()->version;
  CurrentVersion = kmalloc(MAX_KMALLOC_SIZE + 2, GFP_KERNEL);
  Buffer = kmalloc(MAX_KMALLOC_SIZE + 2, GFP_KERNEL);
  memset(Buffer, 0, MAX_KMALLOC_SIZE + 2);
  memset(Buffer, 0, MAX_KMALLOC_SIZE + 2);
  
  if (CurrentVersion)
    strncpy(CurrentVersion, KernelVersion, MAX_KMALLOC_SIZE);
  else
    {
      pr_info("%s: Init failure : Could not allocate memory\n", MODULE_NAME);
      return (-1);
    }
  
  if (misc_register(&mdev) == 0)
    {
      
      pr_info("%s: Kernel version is '%s'\n", MODULE_NAME, KernelVersion);
      return 0;
    }
  else
    {
      return -1;
    }
}

void my_exit_module(void)
{
  pr_info("%s: Goodbye from module !\n", MODULE_NAME);

  misc_deregister(&mdev);
  kfree(CurrentVersion);
  kfree(Buffer);
}

static long mylkm_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
  //pr_info("%s: Debug, ioctl_num : %u\n", MODULE_NAME, ioctl_num);

  switch (ioctl_num)
    {
    case VERSION_MODIFIED_CODE:
      if (strcmp(CurrentVersion, KernelVersion) == 0)
	return IS_NOT_MODIFIED;
      else
	return IS_MODIFIED;
      
    case VERSION_RESET_CODE:
      {
	memset(CurrentVersion, 0, MAX_KMALLOC_SIZE + 1);
	strncpy(CurrentVersion, KernelVersion, (strlen(KernelVersion) > MAX_KMALLOC_SIZE ? MAX_KMALLOC_SIZE : strlen(KernelVersion)));
	return 0;
      }

    default:
      break;
    }
  return (-EINVAL);
}

static ssize_t mylkm_read (struct file *file, char __user *buffer, size_t length, loff_t *offset)
{
  int lengthCurrentVersion = strlen(CurrentVersion);
  int nbNotCopied;
  
  if (length >= lengthCurrentVersion)
  {
    nbNotCopied = copy_to_user(buffer, CurrentVersion, lengthCurrentVersion);
    if (nbNotCopied > 0)
      return (lengthCurrentVersion - nbNotCopied);
    else if (nbNotCopied == 0)
      return (lengthCurrentVersion);
    else
      return (-EFAULT);
  }
  else
    {
      nbNotCopied = copy_to_user(buffer, CurrentVersion, length);
      //pr_info("%s: %s nbNotCopied = %d, length = %lu\n", MODULE_NAME, __func__, nbNotCopied, length);
      //pr_info("%s: %s buffer = %s\n", MODULE_NAME, __func__, buffer);
      
      if (nbNotCopied > 0)
	return (length - nbNotCopied);
      else if (nbNotCopied == 0)
	return length;
      else
	return (-EFAULT);
    }
}

static ssize_t mylkm_write (struct file *file, const char __user *buffer, size_t length, loff_t *offset)
{
  int newLength = (length > MAX_KMALLOC_SIZE ? MAX_KMALLOC_SIZE : length);
  int ret;

  // Next line is because otherwise, CurrentVersion is erased, even if buffer is clearly invalid.
  // (Or maybe I forgot how to code in C. Too much C#)
  ret = copy_from_user(Buffer, buffer, newLength);
  if (ret == newLength)
    return (-EFAULT);
  else
    {
      Buffer[newLength] = '\0';
      memcpy(CurrentVersion, Buffer, strlen(Buffer));
      CurrentVersion[newLength] = '\0';
    }
  return (newLength - ret);
}

module_init(my_init_module);
module_exit(my_exit_module);
