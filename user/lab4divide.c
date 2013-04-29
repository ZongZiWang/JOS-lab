// test divide handler -- divide by zero

#include <inc/lib.h>
#include <inc/challenge.h>

void
handler(struct UTrapframe *utf)
{
	cprintf("%Cshilab4 challenge4: divide by zero! During the handler by User Mode\n");
	cprintf("The answer supposes to be 'Infinitive', but the machine's generated error answer is: ");
}

void
umain(void)
{
#ifdef LAB4_CHALLENGE4
	set_divide_handler(handler);
#endif
	int x = 0xffffffff;
	int y = 0;
	cprintf("%d\n", x/y);
	cprintf("===Pragram is still continuing!===\n");
}
