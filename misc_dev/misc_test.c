#include <linux/miscdevice.h>
#include <linux/init.h>
#include <linux/module.h>
#include<linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("chewei");

static int misc_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk ("misc ioctl\n");
	return 0;
}

static int misc_open(struct inode *inode, struct file *filp)
{
	printk ("misc open\n");
	return 0;
}
static int misc_release(struct inode *inode, struct file *filp)
{
	printk ("misc release\n");
	return 0;
}

static ssize_t misc_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
      printk ("misc read\n");
	  return 0;
}

static ssize_t misc_write(struct file *filp, const char *buf, size_t size, loff_t *f_pos) {

     printk("misc write\n");

return size;
}



static struct file_operations misc_fops =
{
	.owner   = THIS_MODULE,
	.read    = misc_read,
	.open    = misc_open,
    .write =misc_write,
	.release = misc_release,
   // .ioctl = misc_ioctl,
    .unlocked_ioctl = misc_ioctl,
};

static struct miscdevice misc_test = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "misc_test", //此名称将显示在/dev目录下面
	.fops = &misc_fops,
};

static int __init dev_init(void)
{
	int ret = misc_register(&misc_test);
	printk ("misc test initialized\n");
	return ret;
}

static void __exit dev_exit(void)
{
	misc_deregister(&misc_test);
	printk("misc test unloaded\n");
}

module_init(dev_init);
module_exit(dev_exit);


