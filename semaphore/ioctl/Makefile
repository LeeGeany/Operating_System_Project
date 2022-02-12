KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := ioctl.o

PWD := $(shell pwd)

all:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	make -C $(KERNELDIR) M=$(PWD) clean
