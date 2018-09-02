== proc interface usage example ==

$ tree
.
├── hello_ioctl
│   ├── hello_ioctl
│   └── hello_ioctl.c
├── hello_world.c
└── Makefile

# compile kernel-module

	make

# insert kernel module into the Linux kernel
	
	sudo insmod hello_world.ko

# check whether module is loaded
	
	lsmod | grep hello

# see initial variables in kernel-space via /proc interface
	
	cat /proc/hello_proc

# set/get variables in kernel-space via ioctl() calls from user-space
	
	cd hello_ioctl

	./hello_ioctl

# observe changed variable via /proc interface again

	cat /proc/hello_proc
