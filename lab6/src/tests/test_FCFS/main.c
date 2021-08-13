#include "../myOS/userInterface.h"   //interface from kernel

#include "shell.h"
#include "memTestCase.h"
#include "../myOS/include/task.h"
#include "test.h"

void wallClock_hook_main(void){
	int _h, _m, _s;
	char hhmmss[]="hh:mm:ss\0\0\0\0";

	getWallClock(&_h,&_m,&_s);
	sprintf(hhmmss,"%02d:%02d:%02d",_h,_m,_s);
	put_chars(hhmmss,0x7E,24,72);
}

void doSomeTestBefore(void){		
	setWallClock(18,59,59);		//set time 18:59:59
    	setWallClockHook(&wallClock_hook_main);
}

void myMain(void){    

    clear_screen();

    doSomeTestBefore();
	myPrintf(0x7,"THIS IS TEST FOR FCFS\n");
	createTsk(myTsk0,10,3);
	createTsk(myTsk1,5,15);
	createTsk(myTsk2,15,7);

	initShell();
    memTestCaseInit();
    createTsk(startShell,1,80);

	tskEnd();

	//return;
}
