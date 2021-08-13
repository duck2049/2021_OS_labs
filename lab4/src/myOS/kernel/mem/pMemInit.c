#include "../../include/myPrintk.h"
#include "../../include/mem.h"

#include "../../include/io.h"
unsigned long pMemStart;//可用的内存的起始地址
unsigned long pMemSize;//可用的大小

//内存检测，检测从某一个地址开始的内存是否可用
void memTest(unsigned long start, unsigned long grainSize){
	
	//开始的地址要大于1M，需要做一个if判断
	if(start < 0x100000){
		myPrintk(0x7,"ERROR IN START ADDRESS  \n");
		return;
	}
	//grainsize不能太小，也要做一个if判断
	if(grainSize<0x1000){
		myPrintk(0x7,"THE GRAINSIZE IS TOO SMALL  \n");
		return;
	}

	int flag = 1;//flag=1检测未结束
	unsigned short temp;//临时存储读出来的数据
	unsigned short *addr=start;//检测的地址

	while(flag){
		//读出原来的值
		temp = *addr;
		//test(2,3)
		*addr = 0xAA55;
		if(*addr!=0xAA55){
			flag = 0;
			break;
		}
		*addr = 0x55AA;
		if(*addr!=0x55AA){
			flag = 0;
			break;
		}
		//写回原来的值
		*addr = temp;
		//以grainSize为步长检测
		addr += grainSize / sizeof(unsigned short);
	}

	//最后，输出可用的内存的起始地址和大小，别忘记赋值给上面的全局变量
	pMemStart = start;
	pMemSize = (unsigned long)addr - start;
	myPrintk(0x7, "MemStart: %x  \n", pMemStart);
	myPrintk(0x7,"MemSize:  %x  \n",pMemSize);
}

extern unsigned long _end;
void pMemInit(void){
	unsigned long _end_addr = (unsigned long) &_end;
	memTest(0x100000,0x1000);
	myPrintk(0x7,"_end:  %x  \n", _end_addr);
	if (pMemStart <= _end_addr) {
		pMemSize -= _end_addr - pMemStart;
		pMemStart = _end_addr;
	}

	pMemHandler = dPartitionInit(pMemStart,pMemSize);	
}
