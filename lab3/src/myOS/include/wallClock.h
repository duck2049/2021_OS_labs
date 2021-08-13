#ifndef __WALLCLOCK_H__
#define __WALLCLOCK_H__

void updateWallClock(void);
void displayWallClock(void);
void setWallClock(int h, int m, int s);
void getWallClock(int* h, int* m, int* s);
void setUpdateWallClockHook(void (*func)(void));
void setDisplayWallClockHook(void (*func)(void));


#endif