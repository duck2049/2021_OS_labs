#ifndef __TASK_H__
#define __TASK_H__

#define TASK_NUM 20
#define STACK_SIZE 1000

#define TASK_READY 1

void initTskBody(void);


void do_CTX_SW(void*prevTSK_STKPtr, void*nextTSK_STKPtr);

//这个是预先引用了user段代码里面的myMain，方便调用
extern void myMain(void);

//这个是定义了myTCB结构体，不仅包括了TCB声明时得到的栈空间的头地址，
//还包括了此task当前的栈顶地址(栈顶指针会随着若干次pop和push后会变化)
//还包括了分配的pid以及作为链表节点用的next与pre两个指针。
struct myTCB;

struct myTCB {
     unsigned long *stkTop;     /* 栈顶指针 */
     unsigned long *stack_head;
     int pid;
     int task_status;
     int pri;
     int time;
     struct myTCB *next;
     struct myTCB *pre;
};

struct scheduler{
     unsigned int type;
     void (*schedule)();
}scheduler_FCFS,scheduler_PRI,scheduler_SJF;

//定义了基于TCB作为节点的双向链表队列类型
struct TCB_QUEUE{
     struct myTCB head,tail;
     int task_count;
}tcb_pool;

struct myTCB * idleTsk;                /* idle 任务 */
struct myTCB * currentTsk;             /* 当前任务 */

//初始化任务管理器 用了其他的函数名
//void TaskManagerInit(void);

//建立新进程
int createTsk(void (*task)(void),int pri,int time);	//使用函数指指针创建TCB 返回进程号

//删除进程对应的tcb
void destroyTsk(int pid);    //按照pid删除对应的tcb

//调度开始运行某个task
void tskStart(struct myTCB *tcb);

//由进程自己调用，currenttsk主动放弃CPU使用权时调用的入口函数
void tskEnd();

//栈是tcb中最重要的部分，用这个程序来完成 :
//1.初始化栈的内容 尤其重要的是把task这个函数指针放到了合适的位置 2.修改栈顶的指针内容（指向加入了若干数据后的栈顶） （所以要用**stk）
void init_stack(unsigned long **stk, void (*task)(void));

//初始化队列
void init_queue();

//初始化任务管理器
void init_task_manager();

//调度下一个任务进入运行
void (*schedule)();

//具体用FCFS实现的调度算法
void schedule_FCFS();
void schedule_PRI();
void schedule_SJF();

//idle任务的函数体
void idle_mainbody();

//开始由单任务引导过程，切换到多任务工作状态，从现在开始运行的所有任务都是由调度器调度出来的
void startMultitask();

void initScheduler();
void getScheduler();

//直接取出FCFS的下一个任务
struct myTCB* nextFCFSTsk();

#endif
