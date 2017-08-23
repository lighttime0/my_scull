obj-m := scull.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.order *.symvers *.mod.c *.mod.o .tmp_versions .scull*
