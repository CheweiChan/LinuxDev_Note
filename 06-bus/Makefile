CONFIG_MODULE_SIG=n

obj-m := driver.o
::obj-m := device.o
::obj-m := bus.o
ifeq ($(KERNELDIR),)
KERNELDIR=/lib/modules/$(shell uname -r)/build
endif

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
