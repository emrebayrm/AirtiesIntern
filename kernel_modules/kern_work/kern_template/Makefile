obj-m += hello_world.o
CC 		:= x86_64-linux-gnu-gcc

all:
	make -C ../linux-stable-4.10.6 SUBDIRS=$(PWD) modules
	make -C user

clean:
	make -C ../linux-stable-4.10.6 SUBDIRS=$(PWD) clean
	make -C user clean
