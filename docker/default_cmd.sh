#!/bin/bash
set -ex

fail() {
    echo $1 >&2
    exit 1
}

# Separate invocations of make could be merged into a single Makefile and use more parallelism.
# However, I'm keeping them separate to reduce output cluttering.
SRC=/home/student/src
if ! cd $SRC/linux ; then
    fail "Mount the linux folder as a volume under $SRC/linux"
fi
./configure.sh
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-

if ! cd $SRC/initramfs ; then
    fail "Mount a volume with initramfs source files under $SRC/initramfs"
fi
make ARCH=arm CROSS_COMPILE=arm-linux-gnueabi-

BOOT=$SRC/linux/linux/arch/arm/boot
qemu-system-arm -M versatilepb -m 128M -nographic -kernel $BOOT/zImage -dtb $BOOT/dts/versatile-pb.dtb -initrd build/rootfs.gz -append "root=/dev/ram"
