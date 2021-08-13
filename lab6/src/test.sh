#!/bin/sh

make clean


echo "TEST FCFS"

rm -rf userApp
mkdir -p userApp
cp tests/test_FCFS/* userApp/

make
if [ $? -ne 0 ]; then
	echo "make failed"
else
	echo "make succeed"
	echo "START TEST FCFS"
	qemu-system-i386 -kernel output/myOS.elf -serial pty 
	read -s -n1 -p "按任意键以继续"
fi

make clean

echo "TEST priority"

rm -rf userApp
mkdir -p userApp
cp tests/test_priority/* userApp/

make
if [ $? -ne 0 ]; then
	echo "make failed"
else
	echo "make succeed"
	echo "START TEST priority"
	qemu-system-i386 -kernel output/myOS.elf -serial pty 
	read -s -n1 -p "按任意键以继续"
fi

make clean

echo "TEST SJF"

rm -rf userApp
mkdir -p userApp
cp tests/test_SJF/* userApp/

make
if [ $? -ne 0 ]; then
	echo "make failed"
else
	echo "make succeed"
	echo "START TEST SJF"
	qemu-system-i386 -kernel output/myOS.elf -serial pty 
fi

