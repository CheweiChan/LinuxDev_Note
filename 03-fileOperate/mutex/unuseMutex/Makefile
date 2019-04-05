CONFIG_MODULE_SIG=n

obj-m := mutex_drv.o

ifeq ($(KERNELDIR),)
KERNELDIR=/lib/modules/$(shell uname -r)/build
endif

all:
	make -C $(KERNELDIR) M=$(PWD) modules
	gcc -o app app.c -lpthread 
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
