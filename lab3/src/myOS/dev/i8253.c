#include "io.h"
void init8253(void) {
	short parameter;
	parameter = 1193182 / 100;//��Ƶ����

	//��ʼ������
	outb(0x43, 0x34);
	outb(0x40, parameter % (1 << 8));//��8λ
	outb(0x40, parameter>>8);//��8λ

	//����8259������ʱ���ж�
	outb(0x21,inb(0x21)&0xFE);

}