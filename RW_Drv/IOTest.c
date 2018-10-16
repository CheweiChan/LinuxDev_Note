#include <linux/init.h>
#include <linux/module.h>
#include<linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#define EXAMPLE_MAJOR 60
#define EXAMPLE_NAME "IOTest"
MODULE_LICENSE("GPL");
int postion=0;
uint8_t byte[10];
int buff_len;

static int example_open(struct inode *inode, struct file *filp) {
    printk("<1>EXAMPLE: open\n");
    return 0;
}

static int example_close(struct inode *inode, struct file *filp) {
    printk("<1>EXAMPLE: close\n");
    return 0;
}

static ssize_t example_read(struct file *filp, char *buf, size_t size, loff_t *f_pos) {
size_t i;
size_t len;
printk("<1>EXAMPLE: read  (size=%zu)buff_len=%d\n", size,buff_len);
if(buff_len < size)
{
len=buff_len;
}
if(copy_to_user(buf,&byte[0+postion],len))
{  
        return -EFAULT;  
}
for(i=0;i<len;i++)
{
     printk("<1>EXAMPLE: resd  (byte[%zu] = %02x)\n", i, (unsigned)byte[i]);
}

if(postion==0)
{
postion+=len;
}
else
{
if(buff_len==0)
{
len=0;
postion=0;
}
else
{
len=size;
postion+=size;
}
}
buff_len=buff_len-len;
    return len;
}



static ssize_t example_write(struct file *filp, const char *buf, size_t size, loff_t *f_pos) {
    printk("<1>EXAMPLE: write  (size=%zu)\n", size);
    for (buff_len = 0; buff_len < size; ++buff_len) {
        if (copy_from_user(&byte[buff_len], buf + buff_len, 1) != 0) {
            break;
        }

        printk("<1>EXAMPLE: write  (byte[%d] = %02x)\n", buff_len,byte[buff_len]);

    }
    return buff_len;
}

static struct file_operations example_fops = {
    .open = example_open,
    .release = example_close,
    .read = example_read,
    .write = example_write,
};


static int example_init(void) {
    int result;
    printk("<1>EXAMPLE: IOTEST init\n");
     printk(KERN_INFO "Create successful!!!\n");
    /* Register character device */
    result = register_chrdev(EXAMPLE_MAJOR, EXAMPLE_NAME, &example_fops);
    if (result < 0) {
        printk("<1>EXAMPLE: Failed to register character device\n");
        return result;
    }

    return 0;
}

static void example_exit(void) {
    printk("<1>EXAMPLE: IOTEST exit\n");

    /* Unregister character device */
    unregister_chrdev(EXAMPLE_MAJOR, EXAMPLE_NAME);
}

module_init(example_init);
module_exit(example_exit);
