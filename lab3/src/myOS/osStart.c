
extern int myPrintk(int color,const char *format, ...);
extern void clear_screen(void);
extern void myMain(void);
extern void init8259A(void);
extern void init8253(void);
extern void setWallClock(int h, int m, int s);
extern void enable_interrupt(void);

void osStart(void){
	clear_screen();
	myPrintk(0x2,"START RUNNING......\n");
	//初始化
	init8259A();
	init8253();
	setWallClock(0, 0, 0);
	enable_interrupt();
	//调用用户态程序
	myMain();
	myPrintk(0x2, "STOP RUNNING......ShutDown\n");
	while(1);
}
