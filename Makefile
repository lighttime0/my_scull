obj-m := scull.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

#make run and make stop need root authority.
run:
	insmod scull.ko
	dmesg | tail -10

stop:
	rmmod scull
	rm /dev/scull

clean:
	rm -rf *.o *.ko *.order *.symvers *.mod.c *.mod.o .tmp_versions .scull*
