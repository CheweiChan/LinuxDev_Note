#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/stat.h>
 
/*create a directory in sys/kobject_test */
MODULE_AUTHOR("chewei");
MODULE_LICENSE("Dual BSD/GPL");
 
void obj_test_release(struct kobject *kobject);
ssize_t kobj_test_show(struct kobject *kobject, struct attribute *attr,char *buf);
ssize_t kobj_test_store(struct kobject *kobject,struct attribute *attr,const char *buf, size_t count);
 
struct attribute test_attr = {
        .name = "kobj_config",//file name
        .mode = S_IRWXUGO,
};
 
static struct attribute *def_attrs[] = {
        &test_attr,
        NULL,
};
 
 
struct sysfs_ops obj_test_sysops =
{
        .show = kobj_test_show,
        .store = kobj_test_store,
};
 
struct kobj_type ktype = 
{
        .release = obj_test_release,
        .sysfs_ops=&obj_test_sysops,
        .default_attrs=def_attrs,
};
 
void obj_test_release(struct kobject *kobject)
{
        printk("eric_test: release .\n");
}
 
ssize_t kobj_test_show(struct kobject *kobject, struct attribute *attr,char *buf)
{
        printk("have show.\n");
        printk("attrname:%s.\n", attr->name);
        sprintf(buf,"%s\n",attr->name);//buf data transfer to userspace
        return strlen(attr->name)+2;
}
 
ssize_t kobj_test_store(struct kobject *kobject,struct attribute *attr,const char *buf, size_t count)
{
        printk("havestore\n");
        printk("write: %s\n",buf);//buf data from userspace
        return count;
}
 
struct kobject kobj;
static int kobj_test_init(void)
{
        printk("kboject test init.\n");
        kobject_init_and_add(&kobj,&ktype,NULL,"kobject_test");//directory name
        return 0;
}
 
static int kobj_test_exit(void)
{
        printk("kobject test exit.\n");
        kobject_del(&kobj);
        return 0;
}
 
module_init(kobj_test_init);
module_exit(kobj_test_exit);
