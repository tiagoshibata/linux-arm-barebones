# Linux ARM barebones

## Cloning

Use `git clone --recursive` to download this repository with submodules.

## Building

Running `docker build -t tiagoshibata/linux-arm-barebones .` at the docker folder builds a container with required tools (including qemu).

## Running

The `./run.sh` script at the repository's root folder runs the container, passing volumes for the Linux kernel and initramfs build folder.
