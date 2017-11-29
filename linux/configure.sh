#!/bin/bash
set -ex
linux="$(dirname "$0")"/linux
cd "$linux"

fail() {
    echo $1 >&2
    exit 1
}

if [ ! -f Makefile ] ; then
	fail "Mount a linux tree volume under $linux (did you forget to clone the linux submodule?)"
fi

if [ ! -f .config ] ; then
    echo ".config not found, using versatile template"
    make ARCH=arm versatile_defconfig
    # Enable debug symbols and devtmpfs support
    sed -i \
        -e 's/.*CONFIG_DEBUG_INFO.*/CONFIG_DEBUG_INFO=y/' \
        -e 's/.*CONFIG_DEVTMPFS.*/CONFIG_DEVTMPFS=y/' .config
    make ARCH=arm olddefconfig
fi
