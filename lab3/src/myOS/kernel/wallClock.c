#include "myPrintk.h"
void updateWallClock(void);
void displayWallClock(void);
void (*updateClockHook)(void)= updateWallClock;
void (*displayClockHook)(void) = displayWallClock;

int ms = 0, ss = 0, mm = 0, hh = 0;
//更新wallclock
void updateWallClock(void) {
	ss++;
	if (ss == 60) {
		ss = 0;
		mm++;
		if (mm == 60) {
			mm = 0;
			hh++;
			if (hh == 24) {
				hh = 0;
			}
		}
	}
}

//wallclock显示
void displayWallClock(void) {
	if (0 <= hh && hh <= 9) {
		put_chars(0xF, 24, 70, "0%d", hh);
	}
	else {
		put_chars(0xF, 24, 70, "%d", hh);
	}
	put_chars(0xF, 24, 72, ":");
	if (0 <= mm && mm <= 9) {
		put_chars(0xF, 24, 73, "0%d", mm);
	}
	else {
		put_chars(0xF, 24, 73, "%d", mm);
	}
	put_chars(0xF, 24, 75, ":");
	if (0 <= ss && ss <= 9) {
		put_chars(0xF, 24, 76, "0%d", ss);
	}
	else {
		put_chars(0xF, 24, 76, "%d", ss);
	}
	
	
}

//设置wallclock
void setWallClock(int h, int m, int s) {
	ss = s;
	mm = m;
	hh = h;

}

//读取wallclock
void getWallClock(int* h, int* m, int* s) {
	*h = hh;
	*m = mm;
	*s = ss;

}

//修改函数指针
void setUpdateWallClockHook(void (*func)(void)) {
	updateClockHook = func;
}

void setDisplayWallClockHook(void (*func)(void)) {
	displayClockHook = func;
}