// program to cause a illegal opcode

#include <inc/lib.h>
#include <inc/challenge.h>

void
handler(struct UTrapframe *utf)
{
	cprintf("%Cshilab4 challenge4: illegal opcode! During the handler by User Mode\n");
}

void
umain(void)
{
#ifdef LAB4_CHALLENGE4
	set_illop_handler(handler);
#endif
	cprintf("Undone!\n");//TODO
	cprintf("===Pragram is still continuing!===\n");
}

