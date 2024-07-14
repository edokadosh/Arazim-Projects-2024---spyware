
obj-m := message_slot.o

KDIR := /usr/src/linux-headers-5.15.0-113-generic/
PWD := $(shell pwd)

all:
		$(MAKE) -C $(KDIR) M=$(PWD) modules

clean: 
		$(MAKE) -C $(KDIR) M=$(PWD) clean
