#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include<linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include<asm/unistd.h>
#include<linux/sched.h>
#include<linux/wait.h>
#include<linux/delay.h>
#define EXAMPLE_NAME "IOTest"
MODULE_LICENSE("GPL");
uint8_t byte[100];
int buff_len;
int havedata=0;
wait_queue_head_t queue_rw;



static int example_open(struct inode *inode, struct file *filp) {
    printk("open\n");
    return 0;
}

static int example_close(struct inode *inode, struct file *filp) {
    printk("close\n");
    return 0;
}

static ssize_t example_read(struct file *filp, char *buf, size_t size, loff_t *f_pos) {
    size_t i;
    size_t len=size; 

    while(!havedata)
    {
        if(filp->f_flags & O_NONBLOCK)
        {
            printk("NOBLOCK\n");
            return -EAGAIN;
        }
        printk("read  size=%d\n",size);
        printk("Block\n");

        wait_event_interruptible(queue_rw,havedata);

        printk("wakeup\n");
    }
    havedata=0;

    if(copy_to_user(buf,&byte[0],len))
    {  
        return -EFAULT;  
    }

    for(i=0;i<buff_len;i++)
    {
       printk("read  (byte[%zu] = %02x)\n", i,byte[i]);
    }
    len=buff_len;
    buff_len=0;
    return len;

}



static ssize_t example_write(struct file *filp, const char *buf, size_t size, loff_t *f_pos) {

    printk("write  (size=%zu)\n", size);
    for (buff_len = 0; buff_len < size; ++buff_len) 
    {
        if (copy_from_user(&byte[buff_len], buf++, 1) != 0) 
            break;  
        printk("write  (byte[%d] = %02x)\n", buff_len,byte[buff_len]);

    }
    havedata=1;
    wake_up(&queue_rw);
    mdelay(200);//write process will be interrupted by read process
    return buff_len;
}

static struct file_operations example_fops = {
    .open = example_open,
    .release = example_close,
    .read = example_read,
    .write = example_write,
};


static struct miscdevice misc_test = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = EXAMPLE_NAME, //此名称将显示在/dev目录下面
	.fops = &example_fops,
};



static int example_init(void) {
    int result;
    /* Register character device */
    result = misc_register(&misc_test);
    if (result < 0) {
        printk("Failed to register character device\n");
        return result;
    }
    printk(KERN_INFO "Create successful!!!\n");
    init_waitqueue_head(&queue_rw);


    return 0;
}

static void example_exit(void) {
    printk("IOTEST exit\n");
    misc_deregister(&misc_test);
}

module_init(example_init);
module_exit(example_exit);
