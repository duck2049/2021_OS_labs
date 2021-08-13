#include "io.h"
void init8253(void) {
	short parameter;
	parameter = 1193182 / 100;//分频参数

	//初始化序列
	outb(0x43, 0x34);
	outb(0x40, parameter % (1 << 8));//低8位
	outb(0x40, parameter>>8);//高8位

	//控制8259，允许时钟中断
	outb(0x21,inb(0x21)&0xFE);

}