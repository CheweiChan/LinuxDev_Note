#include <linux/miscdevice.h>
 #include <linux/delay.h>
 #include <linux/kernel.h>
 #include <linux/module.h>
 #include <linux/init.h>
 #include <linux/mm.h>
 #include <linux/fs.h>
 #include <linux/types.h>
 #include <linux/delay.h>
 #include <linux/moduleparam.h>
 #include <linux/slab.h>
 #include <linux/errno.h>
 #include <linux/ioctl.h>
 #include <linux/cdev.h>
 #include <linux/string.h>
 #include <linux/list.h>
 #include <linux/pci.h>
 #include <linux/gpio.h>

#define DEVICE_NAME "mymap"
 
 static unsigned char *buffer;
 
 
 static int my_open(struct inode *inode, struct file *file)
 {
     return 0;
 }
 
 
 static int my_map(struct file *filp, struct vm_area_struct *vma)
 {    
     unsigned long page;
     unsigned char i;
     unsigned long start = (unsigned long)vma->vm_start;//this is a kernel address,maybe allocate random by kernel.
     unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start);

     //get the buffer kernel address
     page = virt_to_phys(buffer);   
 
     //mapping vma.start to page(buffer) which continous physical memory
     if(remap_pfn_range(vma,start,page>>PAGE_SHIFT,size,PAGE_SHARED))//PAGE_SHIFT = 12(>>12=4096) get page number
	return -1;
 

     
     return 0;
 }
 
 
 static struct file_operations dev_fops = {
     .owner    = THIS_MODULE,
     .open    = my_open,
     .mmap   = my_map,
 };
 
 static struct miscdevice misc = {
     .minor = MISC_DYNAMIC_MINOR,
     .name = DEVICE_NAME,
     .fops = &dev_fops,
 };

static int __init dev_init(void)
 {
     int ret;    
 
     ret = misc_register(&misc);

     buffer = (unsigned char *)kmalloc(PAGE_SIZE,GFP_KERNEL);
     //reserve kernel mermory which size is 4096 byte 
     SetPageReserved(virt_to_page(buffer));
     //write data to share memory
     for(i=0;i<10;i++)
         buffer[i] = i;
 	 printk("insmod mmap\n");
     return ret;
 }
 
 
 static void __exit dev_exit(void)
 {

     misc_deregister(&misc);

     ClearPageReserved(virt_to_page(buffer));

     kfree(buffer);
	 printk("rmmod mmap\n");
 }
 

 module_init(dev_init);
 module_exit(dev_exit);
 MODULE_LICENSE("GPL");


