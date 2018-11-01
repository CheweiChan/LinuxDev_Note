#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/platform_device.h>

MODULE_AUTHOR("David Xie");
MODULE_LICENSE("Dual BSD/GPL");

static struct platform_device *my_device;

static int __init my_device_init(void)
{
	int ret = 0;
        
        /* ����ṹ */
	my_device = platform_device_alloc("my_dev", -1);
        
        /*ע���豸*/
	ret = platform_device_add(my_device);
	
	/*ע��ʧ�ܣ��ͷ�����ڴ�*/
	if (ret)
	    platform_device_put(my_device);
	
	return ret;	
}

static void my_device_exit(void)
{
	platform_device_unregister(my_device);
}

module_init(my_device_init);
module_exit(my_device_exit);
