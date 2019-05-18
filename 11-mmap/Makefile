CONFIG_MODULE_SIG=n

obj-m := mmap_drv.o

ifeq ($(KERNELDIR),)
KERNELDIR=/lib/modules/$(shell uname -r)/build
endif

all:
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc -o app mmap_app.c

clean:
	make -C $(KERNELDIR) M=$(PWD) clean
