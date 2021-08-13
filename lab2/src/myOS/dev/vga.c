/*
 * 本文件实现vga的相关功能，清屏和屏幕输出
 * clear_screen和append2screen必须按照如下借口实现
 * 可以增加其他函数供clear_screen和append2screen调用
 */
extern void outb (unsigned short int port_to, unsigned char value);
extern unsigned char inb(unsigned short int port_from);
#define N 25

int x = 0, y = 0;//x表示列，y表示行

//计算当前读取到的内存位置
short* calc_cord(int x, int y) {
	return (short*)0xB8000 + y * 80 + x;
}


//显示光标
void cursor() {
	outb(0x3D4, 0xE);//选择行
	outb(0x3D5, (y * 80 + x) >> 8);
	outb(0x3D4, 0xF);//选择列
	outb(0x3D5, (y * 80 + x) % (1 << 8));

}


//将屏幕上所有行上移一行，最后一行清空
void add_oneline() {
	/*
	int delta = 80;
	short* ptr, head;
	head = (short*)0xB8000;
	for (ptr = head; ptr != calc_cord(0,N); ptr++) {
		*(ptr) = *(ptr + delta);
	}
	for (; ptr != calc_cord(0,N+1); ptr++) {
		*ptr = 0x0f000f00;
	}
	*/
	for (int i = 0; i < N-1; i++) {//上移
		for (int j = 0; j < 80; j++) {
			(*calc_cord(j,i)) = (*calc_cord(j,i+1));
		}
	}
	for (int i = 0; i < 80; i++) {//最后一行清空
		(*calc_cord(i, N - 1)) = 0x0f00;
	}
	

}


//清屏，即全部清空
void clear_screen(void) {	
	/*满屏4000bytes
	
	int* ptr;
	ptr = (int*)0xB8000;
	for (int i = 1; i <= 1000; i++) {
		*ptr = 0x0f000f00;//黑底白字
		ptr++;
	}
	*/
	for (int i = 0; i < 25; i++) {
		for (int j = 0; j < 80; j++) {
			(*calc_cord(j,i)) = 0x0f00;
		}
	}
	x = y = 0;
	cursor();
}


//VGA输出
void append2screen(char *kBuf,int color){ 
	int j = 0;
	short temp;//存放每一个需要输出的字符以及它的颜色
	while (kBuf[j] != '\0') {
		if (kBuf[j] == '\n') {//对'\n'特判
			j++;
			x = 0;
			y++;
			if (y == N) {//屏满
				add_oneline();
				y = N-1;
			}
		}
		else {
			temp = (short)color << 8 | (short)kBuf[j];//一个2bytes的颜色+字符打包
			j++;
			(*calc_cord(x, y)) = temp;//写入显存
			x++;
			if (x == 80) {//换行
				x = 0;
				y++;
				if (y == N) {//屏满
					add_oneline();
					y = N-1;
				}
			}
		}
	}
	cursor();
}

