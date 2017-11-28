CC=$(CROSS_COMPILE)gcc -Wall -Wextra -Werror -static -g
USR_BIN_FILES:=tree

# End of user configuration

SELF_DIR:=$(dir $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
BUILD_DIR?=build
ROOTFS?=rootfs

# TODO Ideally, use fakeroot during build and set proper directory permissions
$(shell mkdir -p $(BUILD_DIR)/initramfs_root/usr/bin $(BUILD_DIR)/initramfs_root/mnt)

.PHONY: all clean busybox
all: $(BUILD_DIR)/rootfs.gz

clean::
	rm -rf $(BUILD_DIR)
	find $(SELF_DIR) -name '*.o' -delete

include $(SELF_DIR)/usr_bin_binaries.make

$(BUILD_DIR)/rootfs.gz: $(BUILD_DIR)/initramfs_root/init $(USR_BIN) busybox
	cp -ruav $(ROOTFS)/. $(BUILD_DIR)/initramfs_root/
	cd $(BUILD_DIR)/initramfs_root ; find -print0 | cpio -0 -ov -H newc | gzip -9 > ../rootfs.gz

$(BUILD_DIR)/initramfs_root/init: $(SELF_DIR)/src/init.o
	$(CC) -o $@ $^

BUSYBOX=$(SELF_DIR)/busybox
busybox:
	$(BUSYBOX)/configure.sh
	$(MAKE) -C $(BUSYBOX)/busybox
	cp -uav $(BUSYBOX)/busybox/busybox $(BUILD_DIR)/initramfs_root/usr/bin/busybox
