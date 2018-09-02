== initialization ==
cp kern_template /home/stajer/serdar/kern_template
cd kern_template

== build ==
make

== development ==
make
cp hello_world.ko /home/stajer/teeny-linux/initramfs/x86-busybox/lib/modules/
cp user/userapp /home/stajer/teeny-linux/initramfs/x86-busybox/project/userapp
cd /home/stajer/teeny

./run.sh 
qemu-system-x86_64 \
    -kernel obj/linux-x86-basic/arch/x86_64/boot/bzImage \
    -initrd obj/initramfs-busybox-x86.cpio.gz \
    -nographic -append "console=ttyS
