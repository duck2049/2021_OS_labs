.global start

/*set necessary parameters according to multiboot specification*/
MAGIC=0x1BADB002
FLAGS=0x0
CHECKSUM=-(0x0+0x1BADB002)

.section ".multiboot_header"/*multiboot header*/

.long MAGIC
.long FLAGS
.long CHECKSUM

/*the following is the real coding part*/
.text
.code32/*this means 32 bit*/
start:

/*the following part outputs "hello world!", my id and my name in qemu.All characters are printed in white  with green background.*/
movl $0x0f650f68,0xB8000
movl $0x2f6c2f6c,0xB8004
movl $0x2f202f6f,0xB8008
movl $0x2f6f2f77,0xB800c
movl $0x2f6c2f72,0xB8010
movl $0x2f212f64,0xB8014

movl $0x2f202f20,0xB8018/*blanks*/

movl $0x2f422f50,0xB801c
movl $0x2f392f31,0xB8020
movl $0x2f372f30,0xB8024
movl $0x2f342f31,0xB8028
movl $0x2f322f37,0xB802c
movl $0x2f772f5f,0xB8030
movl $0x2f6e2f61,0xB8034
movl $0x2f202f67,0xB8038
movl $0x2f682f73,0xB803c
movl $0x2f202f75,0xB8040


/*the following part outputs "HELLO WORLD" ,my id and my name in terminal.*/
movb $0x48,%al/*output character H */
movw $0x3F8,%dx
outb %al,%dx

movb $0x45,%al/*output character E */
movw $0x3F8,%dx
outb %al,%dx

movb $0x4C,%al/*output character L */
movw $0x3F8,%dx
outb %al,%dx

movb $0x4C,%al/*output character L */
movw $0x3F8,%dx
outb %al,%dx

movb $0x4F,%al/*output character O */
movw $0x3F8,%dx
outb %al,%dx

movb $0x20,%al/*output blank */
movw $0x3F8,%dx
outb %al,%dx

movb $0x57,%al/*output character W*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x4F,%al/*output character O*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x52,%al/*output character R*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x4C,%al/*output character L*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x44,%al/*output character D*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x20,%al/*output BLANK*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x50,%al/*output character P*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x42,%al/*output character B*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x31,%al/*output number 1*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x39,%al/*output number 9*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x30,%al/*output number 0*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x37,%al/*output number 7*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x31,%al/*output number 1*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x34,%al/*output number 4*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x37,%al/*output number 7*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x32,%al/*output number 2*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x57,%al/*output character W*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x41,%al/*output character A*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x4E,%al/*output character N*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x47,%al/*output character G*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x53,%al/*output character S*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x48,%al/*output character H*/
movw $0x3F8,%dx
outb %al,%dx

movb $0x55,%al/*output character U*/
movw $0x3F8,%dx
outb %al,%dx

/*here uses a dead loop to stop the process*/
end:
jmp end

hlt


