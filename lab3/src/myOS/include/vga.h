#ifndef __VGA_H__
#define __VGA_H__

#define N 24

short* calc_cord(int x, int y);
void cursor();
void add_oneline();
void clear_screen(void);
void append2screen(char *kBuf,int color);
void putoutXY(char* pBuf, int color, int row, int col);
void backspaceVGA(void);

#endif
