#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/proc_fs.h>      /* procfs ... */
#include <linux/delay.h>
#include <linux/sched.h>
#include <linux/semaphore.h>

/* simple log */
#define log(fmt, arg...)  printk(KERN_INFO "[semaphore line:%d] "fmt"\n",__LINE__, ##arg);

struct semaphore sem;

static int _open(struct inode *indoe, struct file *filp)
{
        log("open ok");
        return 0;
}

static int _release(struct inode *indoe, struct file *filp)
{
        log("release ok");
        return 0;
}

static ssize_t _read(struct file *filp, char __user *buff, size_t cnt, loff_t *loff)
{
    down(&sem);//--
    log("read ok,sem=%d",sem.count);
    return 0;
}

static ssize_t _write(struct file *filp, const char __user *buff, size_t cnt, loff_t *loff)
{
    up(&sem);//++
    log("write ok,sem=%d",sem.count);
    return cnt;
}



static struct file_operations fourth_drv_fops =
{
        .owner   = THIS_MODULE,
        .open    = _open,
        .release = _release,
        .read    = _read,
        .write   = _write,
};

struct cdev *mydrv;
static dev_t mydevno;
static struct class *myclass;

static int __init mod_init(void)
{
        alloc_chrdev_region(&mydevno, 0, 1, "semaphoreTest");
        mydrv = cdev_alloc();
        cdev_init(mydrv, &fourth_drv_fops); 
        cdev_add(mydrv, mydevno, 1);

        myclass = class_create(THIS_MODULE, "semaphoreTest_class");
        device_create(myclass, NULL, mydevno,NULL,"semaphoreTest");


	    sema_init(&sem,1);
        log("mod_init,sem=%d",sem.count);
        return 0;
}

static void __exit mod_exit(void)
{
        cdev_del(mydrv);
        unregister_chrdev_region(mydevno, 1);
        device_destroy(myclass, mydevno);
        class_destroy(myclass);
        log("mod_exit");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
