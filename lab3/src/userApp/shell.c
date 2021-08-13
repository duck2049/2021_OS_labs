#include "uart.h" 
#include "string.h"
#include "myPrintk.h"

#define COMMAND_NUM 2
extern void backspaceVGA(void);
extern void uart_put_char(unsigned char c);

typedef struct {
	char name[80];
	char help_content[200];
	int (*func)(char* argv);
}myCommand;
myCommand command[COMMAND_NUM];

int solve_cmd(char* argv) {
	for (int j = 0; j < COMMAND_NUM; j++) {
		myPrintk(0xF, command[j].name);
		myPrintk(0xF, "\n");
	}
	return 1;
}
int solve_help(char* str) {
	int i = 0;
	while (str[i] == ' ') {
		i++;
	}
	i += 4;
	while (str[i] == ' ') {
		i++;
	}
	if (str[i] == '\0') {//help
		myPrintk(0xF, command[1].help_content);
		myPrintk(0xF, "\n");
	}
	else if (myStrcmp(&str[i], command[0].name)) {//help command
		myPrintk(0xF, command[0].help_content);
		myPrintk(0xF, "\n");
	}
	else if (myStrcmp(&str[i], command[1].name)) {//help help
		myPrintk(0xF, command[1].help_content);
		myPrintk(0xF, "\n");
	}
	else {
		myPrintk(0xF, "help:no such command");
		myPrintk(0xF, "\n");
	}
	return 1;
}
void initCommand() {
	myStrcpy(command[0].name, "cmd");
	myStrcpy(command[0].help_content, "List all available commands");
	command[0].func = solve_cmd;

	myStrcpy(command[1].name, "help");
	myStrcpy(command[1].help_content, "Usage: help [command].Display help contents about the command");
	command[1].func = solve_help;
}


void solve(char *str) {
	int i = 0;
	while (str[i] == ' ') {
		i++;
	}
	int flag = 0;
	for (int j = 0; j < COMMAND_NUM; j++) {
		if (myStrcmp(&str[i], command[j].name)) {
			command[j].func(str);
			flag = 1;
			break;
		}
	}
	if(flag==0) {
		myPrintk(0xF, "no such command");
		myPrintk(0xF, "\n");
	}
}
void startShell(void) {

	myPrintk(0x2, "WangShu@Desktop:");
	int len=0;
	initCommand();
	while (1) {
		char str[128];
		char ch=uart_get_char();
		if (ch >= 32 && ch <= 126) {
			myPrintk(0xF, "%c", ch);
			str[len] = ch;
			len++;
		}
		else if (ch == 127) {//backspace
			if (len > 0) {
				len--;
				backspaceVGA();
			}
			
		}
		else if (ch == 13) {//\n
			myPrintk(0xF, "\n");
			str[len] = '\0';
			solve(str);
			len = 0;
			myPrintk(0x2, "WangShu@Desktop:");
		}

		
	}
}

