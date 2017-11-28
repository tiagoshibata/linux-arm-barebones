# Create rules to add programs to /usr/bin in the initramfs image.
# Programs to be added are in the USR_BIN_FILES variable. SELF_DIR must have
# been initialized with the files' directory.
# Rules are output in the USR_BIN variable.

USR_BIN:=$(addprefix $(BUILD_DIR)/initramfs_root/usr/bin/,$(USR_BIN_FILES))

$(USR_BIN): $(BUILD_DIR)/initramfs_root/usr/bin/%: $(SELF_DIR)/src/%.o
	$(CC) -o $@ $^
