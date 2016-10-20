ifneq ($(KERNELRELEASE),)
	obj-m := hw1.o
	hw1-y := kernel_module.o some_function.o

else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.o.cmd *.mod.c modules.order Module.symvers .tmp_versions

endif
