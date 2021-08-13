#!/bin/sh
SRC_RT=$(pwd)
#SRC_RT=/home/duck2049/VMShare/lab2/src
echo $SRC_RT

make clean

make

if [ $? -ne 0 ]; then
	echo "make failed"
else
	echo "make succeed"
	qemu-system-i386 -kernel output/myOS.elf -serial pty &
fi
