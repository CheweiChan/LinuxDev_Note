#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>        /* copy_to_user ... */
#include <linux/ioctl.h>        /* ioctl ... */

/* simple log */
#define log(fmt, arg...) printk(KERN_INFO ""fmt"\n", ##arg);

#define MAGIC_NUMBER 'C'
#define _EXECUTE _IO(MAGIC_NUMBER, 1)
#define _WRITE   _IOW(MAGIC_NUMBER, 6, int)
#define _READ    _IOR(MAGIC_NUMBER, 2, int)
#define _RW           _IOWR(MAGIC_NUMBER, 7, int)
#define _IOCTL_MAXNR  8





int _ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    if (_IOC_TYPE(cmd) != MAGIC_NUMBER) {
        log("cmd error");
        return -ENOTTY;
    }
    
    if (_IOC_NR(cmd) > _IOCTL_MAXNR) {
        log("number error");
        return -ENOTTY;
    }
    
    switch(cmd)
    {
        case _EXECUTE:
            log("EXECUTE");
            break;
        case _WRITE:
            log("WRITE");
            break;
        case _READ:
            log("READ");
            break;
        case _RW:
            log("RW");
            break;
        default:
            log("default");
    }
}

static int _open(struct inode *indoe, struct file *filp)
{
    log("file open");
    return 0;
}

static int _release(struct inode *indoe, struct file *filp)
{
    log("file close");
    return 0;
}

static ssize_t _read(struct file *filp, char __user *buff, size_t cnt, loff_t *loff)
{
    log("read ok");
    return 0;
}

static ssize_t _write(struct file *filp, const char __user *buff, size_t cnt, loff_t *loff)
{
    log("write ok");
    return 0;
}

static struct file_operations my_fops =
{
    .owner = THIS_MODULE,
    .open           = _open,
    .release        = _release,
    .read           = _read,
    .write          = _write,
    .unlocked_ioctl = _ioctl,
};

struct cdev *char_drv;            
static dev_t mydevno;           /* device number */
static struct class *myclass;   

static int __init mod_init(void)
{
    /* Get the master number from the system, the specified driver name will be in /proc/device */
    alloc_chrdev_region(&mydevno, 0, 1, "myCharDevice");
    
    /* allocation cdev */
    char_drv = cdev_alloc();
    
    /* init char device,assign file operate struct */
    cdev_init(char_drv, &my_fops);

    
    /* add char device to system */
    cdev_add(char_drv, mydevno, 1);
    
    /* ctreat class directory of "myCharDevice" on /sys/class  */
    myclass = class_create(THIS_MODULE, "myCharDevice");
    
    /* create file node on /dev */
    device_create(myclass, NULL, mydevno,NULL, "myCharDevice");
    
    log("module_init");
    return 0;
}

static void __exit mod_exit(void)
{
    /* rmmod char device */
    cdev_del(char_drv);
    
    /* release device number*/
    unregister_chrdev_region(mydevno, 1);
    
    /* remove class and file node*/
    device_destroy(myclass, mydevno);
    class_destroy(myclass);
    
    log("module_exit");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
