#include "../../include/myPrintk.h"
// 一个EEB表示一个空闲可用的Block
struct EEB {
	unsigned long next_start;
};

void showEEB(struct EEB *eeb){
	myPrintk(0x7,"EEB(start=0x%x, next=0x%x)\n", eeb, eeb->next_start);
}

//eFPartition是表示整个内存的数据结构
struct eFPartition{
	unsigned long totalN;
	unsigned long perSize;  //unit: byte	
	unsigned long firstFree;
};


void showeFPartition(struct eFPartition *efp){
	myPrintk(0x5,"eFPartition(start=0x%x, totalN=0x%x, perSize=0x%x, firstFree=0x%x)\n", efp, efp->totalN, efp->perSize, efp->firstFree);
}

//本函数是为了方便查看和调试的。
void eFPartitionWalkByAddr(unsigned long efpHandler){
	//1、打印eFPartiiton结构体的信息
	struct eFPartition *addr_efp=(struct eFPartition*)efpHandler;
	showeFPartition(addr_efp);
	//2、遍历每一个EEB，打印出他们的地址以及下一个EEB的地址
	struct EEB *p_eeb = (struct EBB *)(addr_efp->firstFree);

	for (unsigned long i=0; i < addr_efp->totalN; i++)
	{
		showEEB(p_eeb);
		p_eeb = (struct EEB *)((unsigned long)p_eeb + addr_efp->perSize);
	}
}

//本函数计算可分配内存的总大小
unsigned long eFPartitionTotalSize(unsigned long perSize, unsigned long n){
	unsigned long real_perSize;
	/*if(perSize % 4 != 0){
		real_perSize = (perSize / 4 + 1) * 4;
	}*/
	//四字节对齐
	real_perSize = ((perSize + 3) >> 2) << 2;
	unsigned long totalSize = real_perSize * n + sizeof(struct eFPartition);
	return totalSize;
}

//本函数初始化内存
unsigned long eFPartitionInit(unsigned long start, unsigned long perSize, unsigned long n){
	
	//第一步需要创建一个eFPartition结构体，需要注意的是结构体的persize不是直接传入的参数perSize，需要对齐。结构体的nextStart也是需要考虑一下结构体本身的大小。
	struct eFPartition *efpHead=(struct eFPartition*)start;
	unsigned long real_persize = ((perSize + 3) >> 2) << 2;
	efpHead->totalN = n;
	efpHead->perSize = real_persize;
	efpHead->firstFree = start + sizeof(struct eFPartition);
	//第二步就是对每一块的内存创建一个EEB，将他们连起来构成一个链。注意最后一块的EEB的nextstart应该是0
	struct EEB *ptr;
	for (unsigned long i = 0; i < n; i++){
		ptr = (struct EEB *)(i * real_persize + efpHead->firstFree);
		ptr->next_start=(struct EEB *)((i+1)* real_persize + efpHead->firstFree);
	}
	ptr->next_start = 0;
	return start;
}

//本函数分配一个空闲块的内存并返回相应的地址，EFPHandler表示整个内存的首地址
unsigned long eFPartitionAlloc(unsigned long EFPHandler){
	struct eFPartition *efpHead = (struct eFPartition *)EFPHandler;
	unsigned long addr = efpHead->firstFree;
	if(addr==0){
		return 0;
	}
	else{
		efpHead->firstFree = ((struct EEB *)addr)->next_start;
	}
	return addr;
}

//回收内存
unsigned long eFPartitionFree(unsigned long EFPHandler,unsigned long mbStart){

	//free掉mbstart之前的内存，mbstart为第一个空闲块的地址。也要更新EBB组成的链表。
	struct eFPartition *efpHead=(struct eFPartition*)EFPHandler;
	//把mbstart回收，插到第一个空闲块前面
	unsigned long old_start=efpHead->firstFree;
	efpHead->firstFree = mbStart;
	((struct EEB *)mbStart)->next_start = old_start;

	return 1;
}
