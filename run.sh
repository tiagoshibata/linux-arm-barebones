#!/bin/bash
set -ex
cd "$(dirname "$0")"
docker run -ti --rm -v $PWD/linux:/home/student/src/linux -v $PWD/initramfs:/home/student/src/initramfs tiagoshibata/linux-arm-barebones
