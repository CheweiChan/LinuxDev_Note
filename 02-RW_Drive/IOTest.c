/*example 
echo "1234567" > /dev/IOTest
cat /dev/IOTest
*/

#include <linux/init.h>
#include <linux/module.h>
#include<linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#define EXAMPLE_MAJOR 60
#define EXAMPLE_NAME "IOTest"
MODULE_LICENSE("GPL");
int postion=0; //每次read時Buff中的位置buff[postion]
uint8_t byte[100];//read buffer
int buff_len;// read時需從buffer要搬多少長度
int buff_pos=0;
static int example_open(struct inode *inode, struct file *filp) {
    printk("<1>EXAMPLE: open\n");
    return 0;
}

static int example_close(struct inode *inode, struct file *filp) {
    printk("<1>EXAMPLE: close\n");
    return 0;
}
// 如果是用cat read 為一個遞迴function會一直到返回值為0 返回值為kernel read len
static ssize_t example_read(struct file *filp, char *buf, size_t size, loff_t *f_pos) {
size_t i;
size_t len; //當次read搬的資料長度  最大為size
printk("<1>EXAMPLE: read  (size=%zu)buff_len=%d\n", size,buff_len);
if(buff_len < size)
{
len=buff_len;
if(len==0)//如果buff沒資料了則重置postion以及返回0 
{
    buff_pos=0;
    postion=0;
    return 0;
}
}
else
{
len=size;
}
    
if(copy_to_user(buf,&byte[0+postion],len))//從kernel space複製資料到user space
{  
        return -EFAULT;  
}
for(i=0;i<len;i++)
{
     printk("<1>EXAMPLE: resd  (byte[%zu] = %02x)\n", i, (unsigned)byte[i]);
}
#if 0
if(postion==0)//第一次讀取從buffer位置0開始
{
postion+=len;//之後位置為postion+=len
}
else//第二次讀取
{
if(buff_len==0)//buff_len=0則讀取完畢
{
len=0;//kernel需要讀取幾個資料
postion=0;
}
else//buff_lem 不為0則繼續讀取
{
len=size;
postion+=size;
}
}
#endif
postion+=size;
buff_len=buff_len-len;//剩餘長度=減掉這次讀出去的資料長度
return len;//kernel 需要讀取的長度 （最後必須返回0讀取才會結束）
}



static ssize_t example_write(struct file *filp, const char *buf, size_t size, loff_t *f_pos) {
    printk("<1>EXAMPLE: write  (size=%zu)\n", size);
    for (buff_len = buff_pos; buff_len < size+buff_pos; ++buff_len) {
        if (copy_from_user(&byte[buff_len], buf++, 1) != 0) {
            break;
        }

        printk("<1>EXAMPLE: write  (byte[%d] = %02x)\n", buff_len,byte[buff_len]);

    }
buff_pos=buff_len;
    return size;
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
