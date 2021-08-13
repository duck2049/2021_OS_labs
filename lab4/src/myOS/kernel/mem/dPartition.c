#include "../../include/myPrintk.h"


//dPartition 是整个动态分区内存的数据结构
struct dPartition{
	unsigned long size;//整个可用内存大小
	unsigned long firstFreeStart; 
};

void showdPartition(struct dPartition *dp){
	myPrintk(0x5,"dPartition(start=0x%x, size=0x%x, firstFreeStart=0x%x)\n", dp, dp->size,dp->firstFreeStart);
}


// EMB每一个block的数据结构，userdata可以暂时不用管。
struct EMB{
	unsigned long size;
	union {
		unsigned long nextStart;    // if free: pointer to next block
        unsigned long userData;		// if allocated, blongs to user
	};	                           
};

void showEMB(struct EMB * emb){
	myPrintk(0x3,"EMB(start=0x%x, size=0x%x, nextStart=0x%x)\n", emb, emb->size, emb->nextStart);
}


//初始化内存
//start:start addr
unsigned long dPartitionInit(unsigned long start, unsigned long totalSize){
	struct dPartition *dpHandler;
	struct EMB *firstEMB;
	//注意有两个地方的大小问题，第一个是由于内存肯定要有一个EMB和一个dP，totalSize肯定要比这两个加起来大不能太小。
	if(totalSize<sizeof(struct EMB)+sizeof(struct dPartition)){
		myPrintk(0x3, "TOTAL SIZE IS TOO SMALL\n");
		return 0;
	}
	//在地址start处，建立dP结构体表示整个数据结构。
	dpHandler = (struct dPartition *)start;
	dpHandler->size = totalSize;
	dpHandler->firstFreeStart = (unsigned long)(dpHandler + 1);//EMB在内存中紧紧跟在dP后面，然后dP的firstFreeStart指向EMB
	//然后，一整块的EMB被分配（以后使用内存会逐渐拆分）
	firstEMB = (struct EMB *)(dpHandler->firstFreeStart);
	firstEMB->size = totalSize - sizeof(struct dPartition);//第二个注意EMB的可以空间不是totalsize，因为dP要占空间
	firstEMB->nextStart = 0;
	//返回start首地址。
	return (unsigned long)dpHandler;
}

//本函数遍历输出EMB 方便调试
void dPartitionWalkByAddr(unsigned long dp){
	//先打印dP的信息，可调用上面的showdPartition。
	showdPartition((struct dPartition*)dp);
	//然后按地址的大小遍历EMB，对于每一个EMB，可以调用上面的showEMB输出其信息
	unsigned long p = (unsigned long)(((struct dPartition*)dp) + 1);//跨过第一个结构体到达EMB开始的地址
	while(p<((struct dPartition*)dp)->size + dp){
		showEMB((struct EMB*)p);
		p = p + ((struct EMB *)p)->size;
	}
}

//=================firstfit, order: address, low-->high=====================
/**
 * return value: addr (without overhead, can directly used by user)
 * failed:return 0
**/
unsigned long dPartitionAllocFirstFit(unsigned long dp, unsigned long size){
	//下面处理block的大小
	//实际分配的block的大小和传入的参数size是不一样的
	unsigned long blockSize = size + sizeof(struct EMB);
	blockSize = ((blockSize + 3) >> 2) << 2; //四字节对齐
	//它最小值不是0，因为EMB数据结构有大小
	if(blockSize < sizeof(struct EMB)){
		blockSize = sizeof(struct EMB);
	}

	//先找first fit
	struct dPartition *dpHead=(struct dPartition *)dp;
	int find = 0;
	unsigned long pre=0, next=0;//作为遍历时记录方便合并
	unsigned long cur = dpHead->firstFreeStart;//正在访问的变量
	
	while (!find)
	{
		if(cur==0){//遍历到了最后一个结束，仍然没有找到
			find = 0;
			break;
		}
		if(((struct EMB*)cur)->size >= blockSize){//找到了合适的block
			find = 1;
			break;
		}
		pre = cur;
		cur = ((struct EMB *)cur)->nextStart;
	}
	if(find==0){
		return 0;
	}
	else{
		next = ((struct EMB *)cur)->nextStart;
		unsigned long size_remained;
		size_remained = ((struct EMB *)cur)->size - blockSize;
		//切割
		unsigned long new_cur = cur + blockSize;//切割后剩下部分的新head
		((struct EMB *)new_cur)->size = size_remained;
		((struct EMB *)cur)->size = blockSize;
		//前后连接
		((struct EMB *)new_cur)->nextStart = next;
		if(pre!=0){
			((struct EMB*)pre)->nextStart = new_cur;
		}
		else{
			dpHead->firstFreeStart = new_cur;
		}
	}
	return cur + sizeof(struct EMB);//返回用户可以使用的内存地址，在EMB块之后
}

unsigned long getEnd(struct EMB* ptr){
	return ptr->size + (unsigned long)ptr;
}

/*
 *return value:  0:error   1:OK
 */
unsigned long dPartitionFreeFirstFit(unsigned long dp, unsigned long start){
	
	struct dPartition *dpHead = (struct dPartition *)dp;
	unsigned long real_start = start - sizeof(struct EMB);
	unsigned long end = real_start + ((struct EMB *)real_start)->size;
	//检查要释放的start~end这个范围是否在dp有效分配范围内
	if(real_start<(unsigned long )(dpHead+1)){
		return 0;
	}
	if(end > dp + dpHead->size){
		return 0;
	}
	//找到合适的位置插入链表，并合并（如果内存地址连续）
	// unsigned long pre = 0, next = dpHead->firstFreeStart;
	// unsigned long pre_end = pre + (unsigned long)((struct EMB *)pre)->size;
	
	struct EMB *cur=(struct EMB*)real_start,*pre = (struct EMB*)0, *next = (struct EMB *)dpHead->firstFreeStart;
	while(1){//找到对应的位置
		if((pre==0 && real_start<= (unsigned long)next) || next==0){
			break;
		}

		if(getEnd(pre)<=real_start && real_start<= (unsigned long)next){
			break;
		}

		pre = next;
		next = next->nextStart;
	}
	if(pre==0){//第一个
		dpHead->firstFreeStart = real_start;
		cur->nextStart = next;
		if(getEnd(cur)==(unsigned long)next){
			cur->nextStart = next->nextStart;
			cur->size += next->size;
		}
	}
	else if (next==0){//最后一个
		pre->nextStart = cur;
		cur->nextStart = 0;
		if(getEnd(pre)==(unsigned long)cur){
			pre->nextStart = 0;
			pre->size += cur->size;
		}
	}
	else{
		pre->nextStart = cur;
		cur->nextStart = next;
		if(getEnd(cur)==(unsigned long)next){
			cur->nextStart = next->nextStart;
			cur->size += next->size;
		}
		if(getEnd(pre)==(unsigned long)cur){
			pre->nextStart = cur->nextStart;
			pre->size += cur->size;
		}
	}
	return 1;
}

//wrap: we select firstFit, you can select another one
//user need not know this
unsigned long dPartitionAlloc(unsigned long dp, unsigned long size){
	return dPartitionAllocFirstFit(dp,size);
}

unsigned long dPartitionFree(unsigned long	 dp, unsigned long start){
	return dPartitionFreeFirstFit(dp,start);
}
