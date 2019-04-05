#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>        /* copy_to_user ... */
#include <linux/ioctl.h>        /* ioctl ... */
#include <linux/slab.h>         /* kmalloc ... */
#include <linux/proc_fs.h>      /* procfs ... */
#include <linux/delay.h>
#include <linux/sched.h>

#include <linux/semaphore.h>

/* simple log */
#define log(fmt, arg...)  printk(KERN_INFO "[mutexTest line:%d] "fmt"\n",__LINE__, ##arg);

#define MEM_SIZE (128)
struct share {
        unsigned char mem[MEM_SIZE];  // share mem
};

struct share *sh;
struct mutex share_mutex;




static void _delay(int time)
{
    int i,j;
    for(i=0;i<time;i++)
        for(j=0;j<1000000000;j++)
        {
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
            asm volatile ("nop");
        }
}
static int _open(struct inode *indoe, struct file *filp)
{
        log("open");
        return 0;
}

static int _release(struct inode *indoe, struct file *filp)
{
        log("close");
        return 0;
}

static ssize_t _read(struct file *filp, char __user *buff, size_t cnt, loff_t *loff)
{
        unsigned long p = *loff;
        unsigned int count = cnt;
        int ret = 0;
        if(p >= MEM_SIZE)
                return 0;
        if(count > MEM_SIZE - p)
                count = MEM_SIZE - p;

       // mutex_lock(&share_mutex);
        if(copy_to_user(buff, sh->mem + p, count))
                ret = -EFAULT;
        else
        {
                *loff += count;
                ret = count;
                log("read size = %d", count);
        }
       // mutex_unlock(&share_mutex);

        log("read ok");
        return ret;
}

static ssize_t _write(struct file *filp, const char __user *buff, size_t cnt, loff_t *loff)
{
        unsigned long p = *loff;
        unsigned int count = cnt;
        int ret = 0;
        int i = 0;

        static int writecount = 0;
        writecount++;

        if(p >= MEM_SIZE)
                return 0;
        if(count > MEM_SIZE - p)
                count = MEM_SIZE - p;

        //mutex_lock(&share_mutex);
        log("writecount = %d", writecount);
        if(writecount % 2 == 1)
        {
            log("thread 0 start");
            for(i = 0; i < count; i++)
            {
			    if(copy_from_user(sh->mem +p + i , buff++, 1))
				    ret = -EFAULT;
                log("[thread 0]write = %d", i);        
                _delay(5);
                schedule();
            }
            log("thread 0 end");
        }
        else
        {
            log("thread 1 start");
            for(i = 0; i < count; i++)
            {
			    if(copy_from_user(sh->mem +p + i , buff++, 1))
				    ret = -EFAULT;
                log("[thread 1]write = %d", i);
                _delay(3);
                schedule();
            }
            log("thread 1 end");
        }
       // mutex_unlock(&share_mutex);

        *loff += count;
        ret = count;
        log("write size = %d", count);

        log("write ok");
        return ret;
}


static loff_t _llseek(struct file *filp, loff_t offset, int whence)
{
        loff_t new_pos = 0;
        switch(whence)
        {
                case 0: /* SEEK_SET */
                        new_pos = offset;
                        break;
 
                case 1: /* SEEK_CUR */
                        new_pos = filp->f_pos + offset;
                        break;
 
                case 2: /* SEEK_END */
                        new_pos = MEM_SIZE -1 + offset;
                        break;
 
                default: /* can't happen */
                        return -EINVAL;
        }

        if ((new_pos < 0) || (new_pos > MEM_SIZE))
                return -EINVAL; 
        filp->f_pos = new_pos;
        return new_pos;
}

static struct file_operations fourth_drv_fops =
{
        .owner   = THIS_MODULE,
        .open    = _open,
        .llseek  = _llseek,
        .release = _release,
        .read    = _read,
        .write   = _write,
};

struct cdev *mydrv;
static dev_t mydevno;
static struct class *myclass;

static int __init mod_init(void)
{
        alloc_chrdev_region(&mydevno, 0, 1, "mutexTest");
        mydrv = cdev_alloc();
        cdev_init(mydrv, &fourth_drv_fops); 
        cdev_add(mydrv, mydevno, 1);

        myclass = class_create(THIS_MODULE, "mutexTest_class");
        device_create(myclass, NULL, mydevno,NULL,"mutexTest");

        sh = (struct share *)kzalloc(MEM_SIZE, GFP_KERNEL);
        if (sh == NULL) {
                kfree(sh);
                return -ENOBUFS;
        }

        mutex_init(&share_mutex);

        log("module_init");
        return 0;
}

static void __exit mod_exit(void)
{
        cdev_del(mydrv);
        unregister_chrdev_region(mydevno, 1);
        device_destroy(myclass, mydevno);
        class_destroy(myclass);
        log("module_exit");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
