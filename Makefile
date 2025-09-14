KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
obj-m := msgdriver.o

all: module user

module:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

user:
	gcc -o writer writer.c
	gcc -o reader reader.c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	-rm -f writer reader
