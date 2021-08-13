#define WHITE 0x7
#include "../myOS/include/task.h"

extern int myPrintf(int color,const char *format, ...);

char *message1 = "********************************\n";

void myTsk0(void){      
	myPrintf(WHITE, message1);
	myPrintf(WHITE, "* Tsk0: HELLO WORLD!  PRI=10 TIME=3 *\n");
	myPrintf(WHITE, message1);
	
	tskEnd();   //the task is end
}

void myTsk1(void){
	myPrintf(WHITE, message1);
	myPrintf(WHITE, "* Tsk1: HELLO WORLD!  PRI=5  TIME=15*\n");
	myPrintf(WHITE, message1);
	
	tskEnd();   //the task is end
}

void myTsk2(void){
	myPrintf(WHITE, message1);
	myPrintf(WHITE, "* Tsk2: HELLO WORLD!  PRI=15 TIME=7 *\n");
	myPrintf(WHITE, message1);
	
	tskEnd();  //the task is end
}