#ifndef __MY_PRINTK_H__
#define __MY_PRINTK_H__

void put_chars(int color, int row, int col, char* fmt, ...);
void myPrintk(int color, char *fmt, ...);
void myPrintf(int color, char *fmt, ...);

#endif