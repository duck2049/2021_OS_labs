#include<stdio.h>
#include<stdarg.h>
#include<malloc.h>

int addchar(char** str,int* strsize, int *strlen,char ch){//用于向字符串添加字符并且检查溢出
	if(*strsize>*strlen){//没有满 
		(*str)[*strlen]=ch;
		(*strlen)++; 
	}else{
		(*strsize)*=2;
		char *tmp=(char*)realloc((*str),*strsize*sizeof(char));
		if(tmp==NULL){//失败了 
			return 0;
		} 
		(*str)=tmp;
		(*str)[*strlen]=ch;
		(*strlen)++; 
	}
	return 1;
}

void printk(char *fmt,...){

	va_list ap;
	int d;
	char c,*s;
	va_start(ap,fmt); 
	
	int strsize=1024;//输出字符串的定义长度 
	int strlen=0;//字符串的已使用长度 
	char *output=(char*)malloc(sizeof(char)*strsize);
	if(output==NULL){//失败了 
		 return; 
	}
	int i=0;
	while(fmt[i]!='\0'){
		if(fmt[i]!='%'){
			addchar(&output,&strsize,&strlen,fmt[i]);
			i++;
		}
		else{//读取到了% 
			i++;
			if(fmt[i]=='\0'){//%\0是不合法的，当他是普通的%添加进去 
				addchar(&output,&strsize,&strlen,'%');
				break;//因为已经读到了\0所以直接结束 
			}else if(fmt[i]=='d'){//读取到了%d 
				int num=va_arg(ap,int);
				int t=1;
				while(t<=num){
					t*=10;
				}
				t/=10;
				while(t>=1){
					addchar(&output,&strsize,&strlen,'0'+(num/t)%10);
					t/=10;
				}
				i++;
				
			}else if(fmt[i]=='c'){//读取到了%c
				char ch=(char)va_arg(ap,int);
				addchar(&output,&strsize,&strlen,ch);
				i++;
			}
		}
	}
	addchar(&output,&strsize,&strlen,'\0');
	
	putvga(char ch);

	int i = 0;
	while (output[i] != '\0') {
		putvga(output[i]);
	}
	
	printf("%s\n",output);//这个是用来输出测试的，请把它改成调用其他函数
}
int main(){
	printk("abc %d kkk %c",123456,'f');
}
