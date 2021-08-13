/*
 * 实现myPrint功能 先学习C语言的可变参数内容
 * 需要调用到格式化输出的function（vsprintf）
 */ 
#include<stdio.h>
#include<stdarg.h>


 
extern void append2screen(char *str,int color);
extern void uart_put_chars(char *str);
extern void clear_screen(void);


char kBuf[400];
void myPrintk(int color, char *fmt, ...){

	//格式化字符串处理
	va_list ap;
	int d;
	char c,*s;
	va_start(ap,fmt); 
	 
	int strlen=0;//字符串的已使用长度 
	int i=0;
	while(fmt[i]!='\0'){
		if(fmt[i]!='%'){
			kBuf[strlen] = fmt[i];
			strlen++;
			i++;
		}
		else{//读取到了% 
			i++;
			if(fmt[i]=='\0'){//%\0是不合法的，当他是普通的%添加进去 
				kBuf[strlen] = '%';
				strlen++; 
				break;//因为已经读到了\0所以直接结束 
			}else if(fmt[i]=='d'){//读取到了%d 
				int num=va_arg(ap,int);//从va_list中切一个int下来
				int t=1;
				if (num == 0) {
					kBuf[strlen] = '0';
					strlen++;

				}
				else if (num < 0) {
					kBuf[strlen] = '-';
					strlen++;
					num *= -1;
					int t = 1;
					//下面将数字转化为字符
					while (t <= num) {
						t *= 10;
					}
					t /= 10;
					while (t >= 1) {
						kBuf[strlen] = '0' + (num / t) % 10;
						strlen++;
						t /= 10;
					}
				}
				else {
					int t = 1;
					while (t <= num) {
						t *= 10;
					}
					t /= 10;
					while (t >= 1) {
						kBuf[strlen] = '0' + (num / t) % 10;
						strlen++;
						t /= 10;
					}
				}
				i++;
				
			}else if(fmt[i]=='c'){//读取到了%c
				char ch=(char)va_arg(ap,int);
				kBuf[strlen] = ch;
				strlen++;
				i++;
			}
		}
	}
	kBuf[strlen] = '\0';
	strlen++;
	va_end(ap);
	
	//调用VGA和串口输出函数
	append2screen(kBuf, color);
	uart_put_chars(kBuf);


	
}

char uBuf[400];
void myPrintf(int color, char *fmt, ...){

	//格式化字符串处理
	va_list ap;
	int d;
	char c, * s;
	va_start(ap, fmt);

	int strlen = 0;//字符串的已使用长度 
	int i = 0;
	while (fmt[i] != '\0') {
		if (fmt[i] != '%') {
			uBuf[strlen] = fmt[i];
			strlen++;
			i++;
		}
		else {//读取到了% 
			i++;
			if (fmt[i] == '\0') {//%\0是不合法的，当他是普通的%添加进去 
				uBuf[strlen] = '%';
				strlen++;
				break;//因为已经读到了\0所以直接结束 
			}
			else if (fmt[i] == 'd') {//读取到了%d 
				int num = va_arg(ap, int);
				if (num == 0) {
					uBuf[strlen] = '0';
					strlen++;

				}
				else if (num < 0) {
					uBuf[strlen] = '-';
					strlen++;
					num *= -1;
					int t = 1;
					while (t <= num) {
						t *= 10;
					}
					t /= 10;
					while (t >= 1) {
						uBuf[strlen] = '0' + (num / t) % 10;
						strlen++;
						t /= 10;
					}
				}
				else {
					int t = 1;
					while (t <= num) {
						t *= 10;
					}
					t /= 10;
					while (t >= 1) {
						uBuf[strlen] = '0' + (num / t) % 10;
						strlen++;
						t /= 10;
					}
				}
				
				i++;

			}
			else if (fmt[i] == 'c') {//读取到了%c
				char ch = (char)va_arg(ap, int);
				uBuf[strlen] = ch;
				strlen++;
				i++;
			}
		}
	}
	uBuf[strlen] = '\0';
	strlen++;
	va_end(ap);

	//调用VGA和串口输出函数
	append2screen(uBuf, color);
	uart_put_chars(uBuf);

}


