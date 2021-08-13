#include "../include/task.h"
#include "../include/kmalloc.h"
#include "../include/myPrintk.h"

#define NULL 0
void init_stack(unsigned long **stk, void (*task)(void))
{
    //这个栈的内容是完全给CTX_SW使用的，下面一一解释
    //这两个数是由ret指令读取的，用来切换eip 也就是instruction pointer
    *(*stk)-- = (unsigned long)0x08;
    *(*stk)-- = (unsigned long)task;
    //这个是给popf来读取的，popf会从栈中取出一个数，写入到flag寄存器，这个寄存器似乎表明了当前task的一些属性
    *(*stk)-- = (unsigned long)0x0202;
    //这个是给popa来读取的，popa会从栈里面取出这些数，写入到通用寄存器
    *(*stk)-- = (unsigned long)0xAAAAAAAA;
    *(*stk)-- = (unsigned long)0xBBBBBBBB;
    *(*stk)-- = (unsigned long)0xCCCCCCCC;
    *(*stk)-- = (unsigned long)0xDDDDDDDD;
    *(*stk)-- = (unsigned long)0x44444444;
    *(*stk)-- = (unsigned long)0x55555555;
    *(*stk)-- = (unsigned long)0x66666666;
    *(*stk)   = (unsigned long)0x77777777;
}

//新建进程
int createTsk(void (*task)(void)){

    struct myTCB *new_TCB = (struct myTCB *)kmalloc(sizeof(new_TCB));
    new_TCB->pid = fcfs_queue.task_count++;
	myPrintk(0x2,"creat new task, pid=%d\n",new_TCB->pid);
    new_TCB->task_status = TASK_READY;
    new_TCB->stack_head = (unsigned long *)kmalloc(sizeof(unsigned long) * STACK_SIZE);
    new_TCB->stkTop = new_TCB->stack_head+(STACK_SIZE-1);
    init_stack(&(new_TCB->stkTop), task);
    new_TCB->next = fcfs_queue.head.next;
    new_TCB->pre = &fcfs_queue.head;
    fcfs_queue.head.next = new_TCB;
    new_TCB->next->pre = new_TCB;
    return new_TCB->pid;
}

//删除tsk
void destroyTsk(int pid){   //按照pid对应的task
    //遍历队列
    struct myTCB *ptr = fcfs_queue.head.next;
    while(ptr!=&fcfs_queue.tail){
        if(ptr->pid==pid){//找到了task
            ptr->next->pre = ptr->pre;
            ptr->pre->next = ptr->next;
            kfree(ptr->stack_head);
            kfree(ptr);
            fcfs_queue.task_count--;
            return;
        }
        ptr = ptr->next;
    }
}

// 调度 让指定进程得到CPU开始运行
void tskStart(struct myTCB *tcb){  
    struct myTCB *pretask = currentTsk;
    currentTsk = tcb;
    do_CTX_SW(&(pretask->stkTop), currentTsk->stkTop);
}

//一个进程停止运行时，删掉他，然后调度下一个进程
void tskEnd(){   
    destroyTsk(currentTsk->pid);
    schedule();
}

//调度下一个进程
void schedule(){
	myPrintk(0x2,"scheduling......\n");
    schedule_FCFS();
}
void schedule_FCFS(){
    struct myTCB *nextTSK = nextFCFSTsk();
    
    //这段代码是把调度出去的进程调到队首，防止其被连续的再次运行 但是我们的进程不重入，所以不必要
    nextTSK->next->pre = nextTSK->pre;
    nextTSK->pre->next = nextTSK->next;

    nextTSK->next = fcfs_queue.head.next;
    nextTSK->next->pre = nextTSK;
    fcfs_queue.head.next = nextTSK;
    nextTSK->pre = &fcfs_queue.head;
    

    tskStart(nextTSK);
}

//取出下一个应该被运行的进程
struct myTCB* nextFCFSTsk(){
    struct myTCB *ptr = fcfs_queue.tail.pre;
    if(ptr!=&fcfs_queue.head){
        
	    //myPrintk(0x2,"FCSF get next task, pid = %d......\n",ptr->pid);
        return ptr;
    }
    return 0;
}

//这两个全局变量实际上没用，只是用来给CTX_SW用来牺牲掉的前一个栈空间
unsigned long BspContextBase[STACK_SIZE];
unsigned long *BspContext;
//start multitasking;
void startMultitask(void){
	myPrintk(0x2,"START Multitask......\n");
    BspContext = BspContextBase + STACK_SIZE - 1;
    currentTsk = nextFCFSTsk();
    do_CTX_SW(&BspContext,currentTsk->stkTop);
}

//初始化任务管理器 要做下面这几个事情
void init_task_manager(){
    init_queue();
    createTsk(myMain);
    createTsk(idle_mainbody);
    startMultitask();
}

//idle的功能代码
void idle_mainbody(){
    while(1){
        myPrintk(0x2,"idle working\n");
        schedule();
    }
}

//初始化队列
void init_queue(){
    fcfs_queue.head.next = &fcfs_queue.tail;
    fcfs_queue.head.pre = NULL;
    fcfs_queue.tail.pre = &fcfs_queue.head;
    fcfs_queue.tail.next = NULL;
    fcfs_queue.task_count = 0;
}

//这两个全局变量被CTX_SW汇编代码直接读取使用，用来实际的传入参数
unsigned long **prevTSK_StackPtr;
unsigned long *nextTSK_StackPtr;

void CTX_SW(void*prevTSK_STKPtr, void*nextTSK_STKPtr);

//这个东西让CTX_SW便于使用，自动的把传入的参数赋值给全局变量，用来和CTX_SW交换数据
void do_CTX_SW(void*prevTSK_STKPtr, void*nextTSK_STKPtr){
    prevTSK_StackPtr = prevTSK_STKPtr;
    nextTSK_StackPtr = nextTSK_STKPtr;
    CTX_SW(prevTSK_StackPtr, nextTSK_StackPtr);
}