CONFIG_MODULE_SIG=n

obj-m := notifier.o

ifeq ($(KERNELDIR),)
KERNELDIR=/lib/modules/$(shell uname -r)/build
endif

all:
	make -C $(KERNELDIR) M=$(PWD) modules

install:
	insmod ./IOTest.ko
	mknod /dev/IOTest c 60 0
Re_install:
	rmmod IOTest
	insmod ./IOTest.ko
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
