extern unsigned char inb(unsigned short int port_from);
extern void outb (unsigned short int port_to, unsigned char value);

#define uart_base 0x3F8

//串口输出单个字符
void uart_put_char(unsigned char c){
    outb(uart_base, c);
}


//单个字符写入
unsigned char uart_get_char(void){
    while (!(inb(uart_base + 5) & 1));
    return inb(uart_base);
              
}

//串口输出字符串
void uart_put_chars(char *str){ 
    int i = 0;
    while (str[i] != '\0') {
        if (str[i] == '\n') {//特判
            uart_put_char('\n');
            uart_put_char('\r');
            i++;
        }
        else {
            uart_put_char(str[i]);
            i++;
        }
    }

}