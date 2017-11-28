#!/bin/bash
set -ex
cd "$(dirname "$0")"/busybox

MAKE="make CROSS_COMPILE=$CROSS_COMPILE"

if [ ! -f .config ] ; then
    echo ".config not found, creating now"
    $MAKE defconfig
    sed -e 's/.*FEATURE_PREFER_APPLETS.*/CONFIG_FEATURE_PREFER_APPLETS=y/' \
        -e 's/.*FEATURE_SH_STANDALONE.*/CONFIG_FEATURE_SH_STANDALONE=y/' \
        -e 's/.*CONFIG_STATIC.*/CONFIG_STATIC=y/' \
        -i .config
fi
