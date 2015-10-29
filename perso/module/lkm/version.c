#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <asm/segment.h>
#include "version.h"
#include <linux/utsname.h>

static int device_open;
static char default_version[__NEW_UTS_LEN];

MODULE_LICENSE("GPL");
MODULE_AUTHOR("caudou_j and rouchy_a");
MODULE_DESCRIPTION("M - Kernel Programming I - LKM");

static int mymisc_open(struct inode *inode, struct file *f)
{
	if (device_open)
		return -EBUSY;
	++device_open;
	pr_info("mymisc_open success\n");
	return 0;
}

static ssize_t mymisc_read(struct file *file,
			   char __user *buf,
			   size_t count,
			   loff_t *off)
{
	int len;

	len = strlen(utsname()->version);
	if (count < len + 1)
		return -EINVAL;
	if (*off != 0)
		return 0;

	if (copy_to_user(buf, utsname()->version, len))
		return -EINVAL;

	if (!access_ok(VERIFY_WRITE, buf + len, 1))
		return -EINVAL;
	buf[len] = '\n';

	*off = len + 1;
	return len + 1;
}

static ssize_t mymisc_write(struct file *file,
			    const char __user *buf,
			    size_t count,
			    loff_t *off)
{
	if (count > __NEW_UTS_LEN - 1)
		count = __NEW_UTS_LEN - 1;

	if (copy_from_user(utsname()->version, buf, count))
		return -EFAULT;

	utsname()->version[count] = '\0';
	return count + 1;
}

static int mymisc_release(struct inode *i, struct file *f)
{
	device_open--;
	return 0;
}

static long mymisc_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int *ret;
	int tmp;

	switch (cmd) {
	case VERSION_MODIFIED:
		if (arg != 0) {
			ret = (int *)arg;
			tmp = strcmp(default_version, utsname()->version);
			if (copy_to_user(ret, &tmp, sizeof(int)))
				return -EINVAL;
		}
		break;
	case VERSION_RESET:
		strcpy(utsname()->version, default_version);
		break;
	}
	return 0;
}

static const struct file_operations mymisc_fops = {
	.owner = THIS_MODULE,
	.read = mymisc_read,
	.write = mymisc_write,
	.open = mymisc_open,
	.release = mymisc_release,
	.unlocked_ioctl = mymisc_ioctl,
};

static struct miscdevice mymisc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "version",
	.fops = &mymisc_fops,
};

static int __init version_init(void)
{
	int ret;

	ret = misc_register(&mymisc);
	if (ret < 0) {
		pr_warn("misc_register() failed: %d\n", ret);
		return ret;
	}

	strcpy(default_version, utsname()->version);
	return 0;
}

static void __exit version_cleanup(void)
{
	int ret;

	strcpy(utsname()->version, default_version);
	ret = misc_deregister(&mymisc);
	if (ret < 0)
		pr_warn("misc_deregister() failed: %d\n", ret);
	else
		pr_info("misc_deregister() success\n");
}

module_init(version_init);
module_exit(version_cleanup);
