extern void myPrintf(int color, char* fmt, ...);
extern void myPrintk(int color, char* fmt, ...);

extern void clear_screen(void);
extern void startShell(void);

void myMain(void){    
    startShell();
    while (1);
    return;
}
