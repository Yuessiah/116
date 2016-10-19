ifneq ($(KERNELRELEASE),)
	obj-m := hw1.o
	obj-m += swap_string.h
	obj-m += sum_tree.h

else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.c modules.order Module.symvers

endif
