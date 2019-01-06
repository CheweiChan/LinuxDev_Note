#include<asm/uaccess.h>
#include<linux/types.h>
#include<linux/kernel.h>
#include<linux/sched.h>
#include<linux/notifier.h>
#include<linux/init.h>
#include<linux/module.h>

static RAW_NOTIFIER_HEAD(myNotifier);//init a raw chain

struct timer_list timer;
unsigned long count=0;

void notifier_function(unsigned long para)//timer function
{
raw_notifier_call_chain(&myNotifier,count,NULL);//transfer count to chain(event)
mod_timer(&timer,jiffies+HZ);//single trigger
count++;
}
//chain 1
static int notifier_event1(struct notifier_block *this,unsigned long event,void *ptr)
{
printk("call event1=%d\n",event);
}
static struct notifier_block notifier1=
{
   .notifier_call=notifier_event1,
};
//chain 2
static int notifier_event2(struct notifier_block *this,unsigned long event,void *ptr)
{
printk("call event2=%d\n",event);
}
static struct notifier_block notifier2=
{
   .notifier_call=notifier_event2,
};
//chain 3
static int notifier_event3(struct notifier_block *this,unsigned long event,void *ptr)
{
printk("call event3=%d\n",event);
}
static struct notifier_block notifier3=
{
   .notifier_call=notifier_event3,
};


int __init myNotifier_init(void)
{
raw_notifier_chain_register(&myNotifier,&notifier1);
raw_notifier_chain_register(&myNotifier,&notifier2);
raw_notifier_chain_register(&myNotifier,&notifier3);
init_timer(&timer);
timer.expires = jiffies+HZ;
timer.function = notifier_function;
add_timer(&timer);

return 0;
}

void __exit myNotifier_exit(void)
{
del_timer(&timer);
raw_notifier_chain_unregister(&myNotifier,&notifier1);
raw_notifier_chain_unregister(&myNotifier,&notifier2);
raw_notifier_chain_unregister(&myNotifier,&notifier3);

}

module_init(myNotifier_init);
module_exit(myNotifier_exit);
MODULE_LICENSE("GPL");








































