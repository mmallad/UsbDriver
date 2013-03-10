obj-m := UsbDriver.o
KD := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
all:
	make -C $(KD) M=$(PWD) modules
clean:
	make -C $(KD) M=$(PWD) clean
