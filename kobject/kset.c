#include <linux/device.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/stat.h>
#include <linux/kobject.h>
 
MODULE_AUTHOR("chewei");
MODULE_LICENSE("Dual BSD/GPL");
 
struct kset kset_p;
struct kset kset_c;

int kset_filter(struct kset *kset, struct kobject *kobj)
{
        printk("Filter: kobj %s.\n",kobj->name);
        return 1;
}
 
const char *kset_name(struct kset *kset, struct kobject *kobj)
{
        static char buf[20];
        printk("Name: kobj %s.\n",kobj->name);
        sprintf(buf,"%s","kset_name");
        return buf;
}
 
int kset_uevent(struct kset *kset, struct kobject *kobj,struct kobj_uevent_env *env)
{
        int i = 0;
        printk("uevent: kobj %s.\n",kobj->name);

        while( i < env->envp_idx){
                printk("%s.\n",env->envp[i]);
                i++;
        }

        return 0;
}

struct kset_uevent_ops uevent_ops = 
{
        .filter = kset_filter,
        .name   = kset_name,
        .uevent = kset_uevent,
};
 
int kset_test_init(void)
{
        printk("kset test init.\n");
        kobject_set_name(&kset_p.kobj,"kset_p");
        kset_p.uevent_ops = &uevent_ops;
        kset_register(&kset_p);
 
        kobject_set_name(&kset_c.kobj,"kset_c");
        kset_c.kobj.kset = &kset_p;
        kset_register(&kset_c);// sys/kset_p/kset_c
        return 0;
}
 
int kset_test_exit(void)
{
        printk("kset test exit.\n");
        kset_unregister(&kset_p);
        kset_unregister(&kset_c);
        return 0;
}
 
module_init(kset_test_init);
module_exit(kset_test_exit);
