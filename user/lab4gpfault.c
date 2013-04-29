// program to cause a genenral protection fault

#include <inc/lib.h>

void
handler(struct UTrapframe *utf)
{
	cprintf("%Cshilab4 challenge4: general protection fault! During the handler by User Mode\n");
}

void
umain(void)
{
#ifdef LAB4_CHALLENGE4
	set_gpfault_handler(handler);
#endif
	asm volatile("int $14");
	cprintf("===Pragram is still continuing!===\n");
}

