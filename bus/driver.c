#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>

#include <linux/miscdevice.h>
#include <linux/init.h>
#include <linux/module.h>
#include<linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
MODULE_AUTHOR("chewei");
MODULE_LICENSE("Dual BSD/GPL");

extern struct bus_type my_bus_type;


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

static ssize_t misc_read(struct file *filp, char *buf, size_t size, loff_t *f_pos) 
{

printk("read\n");

return 0;
}

static ssize_t misc_write(struct file *filp, const char *buf, size_t size, loff_t *f_pos) {

printk("write\n");

return size;
}


static struct file_operations misc_fops =
{
	.owner   = THIS_MODULE,
	.open    = misc_open,
	.release = misc_release,
    .read =misc_read,
    .write = misc_write,
};

static struct miscdevice misc_321 = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "misc_321",
	.fops = &misc_fops,
};

static int my_probe(struct device *dev)
{
int ret;
 ret= misc_register(&misc_321);
    printk("Driver found device which my driver can handle ret=%d!\n",ret);
    return 0;
}

static int my_remove(struct device *dev)
{
    misc_deregister(&misc_321);
    printk("Driver found device unpluged!\n");
    return 0;
}

struct device_driver my_driver = {
	.name = "my_dev",
	.bus = &my_bus_type,
	.probe = my_probe,
        .remove	= my_remove,
};

/*
 * Export a simple attribute.
 */
static ssize_t mydriver_show(struct device_driver *driver, char *buf)
{
	return sprintf(buf, "%s\n", "This is my driver!");
}

static DRIVER_ATTR(drv, S_IRUGO, mydriver_show, NULL);

static int __init my_driver_init(void)
{
	int ret = 0;
        
	driver_register(&my_driver);
		
	driver_create_file(&my_driver, &driver_attr_drv);
	
	return ret;	

}

static void my_driver_exit(void)
{
	driver_unregister(&my_driver);
}

module_init(my_driver_init);
module_exit(my_driver_exit);
