#!/bin/sh
TOP=$HOME/teeny-linux
cd initramfs/x86-busybox
find . -print0 \
	| cpio --null -ov --format=newc \
	| gzip -9 > $TOP/obj/initramfs-busybox-x86.cpio.gz
cd ../..
