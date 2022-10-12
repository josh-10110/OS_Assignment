MODULE_NAME = memdrv

obj-m	:= $(MODULE_NAME).o

KDIR	:= /lib/modules/$(shell uname -r)/build
PWD	:= $(shell pwd)

all: module test_device

module:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

retrieve-prog: retrieve.c libmemdrv.c libmemdrv.h
	gcc -g -W -Wall -pedantic retrieve retrieve.c libmemdrv.c

store-prog: store.c libmemdrv.c libmemdrv.h
	gcc -g -W -Wall -pedantic store store.c libmemdrv.c

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -vf test_device
