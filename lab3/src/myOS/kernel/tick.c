#include "myPrintk.h"
extern void (*displayClockHook)(void);
extern void (*updateClockHook)(void);
extern int ms;
int cnt_tick=0;
void tick(void) {
	ms = cnt_tick / 10;
	cnt_tick++;
	if (cnt_tick % 100 == 0) {
		updateClockHook();
		displayClockHook();

	}

}