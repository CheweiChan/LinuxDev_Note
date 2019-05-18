#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>        /* copy_to_user ... */
#include <linux/ioctl.h>        /* ioctl ... */

/* simple log */
#define log(fmt, arg...)   printk(KERN_INFO "[%s][%d] "fmt"\n", __func__, __LINE__, ##arg);

#define FIRST_IOC_MAGIC 'F'
#define FIRST_ONLY_EXECUTE _IO(FIRST_IOC_MAGIC, 0)
#define FIRST_ONLY_WRITE   _IOW(FIRST_IOC_MAGIC, 1, int)
#define FIRST_ONLY_READ    _IOR(FIRST_IOC_MAGIC, 2, int)
#define FIRST_RW           _IOWR(FIRST_IOC_MAGIC, 3, int)
#define FIRST_IOCTL_MAXNR  3

#define	DEV_MAJOR	60
#define	DEV_NAME	"RWTest"	

int first_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
        if (_IOC_TYPE(cmd) != FIRST_IOC_MAGIC) {
                log("error1");
                return -ENOTTY;
        }

        if (_IOC_NR(cmd) > FIRST_IOCTL_MAXNR) {
                log("error2");
                return -ENOTTY;
        }

        switch(cmd)
        {
                case FIRST_ONLY_EXECUTE:
                        log("FIRST_ONLY_EXECUTE");
                        break;
                case FIRST_ONLY_WRITE:
                        log("FIRST_ONLY_WRITE");
                        break;
                case FIRST_ONLY_READ:
                        log("FIRST_ONLY_READ");
                        break;
                case FIRST_RW:
                        log("FIRST_RW");
                        break;
                default:
                        log("default");
        }
}

static int first_open(struct inode *indoe, struct file *filp)
{
        log("first_drv open ok");
        return 0;
}

static int first_release(struct inode *indoe, struct file *filp)
{
        log("first_drv release ok");
        return 0;
}

static ssize_t first_read(struct file *filp, char __user *buff, size_t cnt, loff_t *loff)
{
        log("first_drv read ok");
        return 0;
}

static ssize_t first_write(struct file *filp, const char __user *buff, size_t cnt, loff_t *loff)
{
        log("first_drv write ok");
        return 0;
}

static struct file_operations file_drv_fops =
{
        .open           = first_open,
        .release        = first_release,
        .read           = first_read,
        .write          = first_write,
        .unlocked_ioctl = first_ioctl,
};

struct cdev *first_drv;                      
static struct class *file_drv_class;   
static dev_t   dev_num;
static int __init mod_init(void)
{
    int major=0;

    major = register_chrdev(dev_num,DEV_NAME, &file_drv_fops); 


    /* ¦b /sys/class RWTest */
    file_drv_class = class_create(THIS_MODULE, DEV_NAME);

    /* ¦b /dev/RWTest */
    device_create(file_drv_class, NULL, MKDEV(major, 0),NULL,DEV_NAME);

    log("mod_init ok");
    return 0;
}

static void __exit mod_exit(void)
{
    
    unregister_chrdev_region(first_drv_devno, 1);

    device_destroy(first_drv_class, first_drv_devno);
    class_destroy(first_drv_class);

    log("mod_exit ok");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
