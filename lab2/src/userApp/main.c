/* 
 * 请确保以下的代码可以原封不动的成功编译和执行
 * 助教测试时会替换相应的参数
 * 不需要修改
 */
extern void myPrintf(int color, char* fmt, ...);
extern void myPrintk(int color, char* fmt, ...);

extern void clear_screen(void);

void myMain(void){    
    int i;
    
    myPrintk(0x7,"main,PB19071472_wangshu\n");
    myPrintf(0x8,"test\n");
    myPrintf(0x8&0x0,"test2\n");
    myPrintf(0x8&0x1,"test2\n");
    for (i=1;i<30;i++) myPrintf(i,"%d  ",i);
    
    
    return;
}
