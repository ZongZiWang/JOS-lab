#include <inc/lib.h>

void
handler(struct UTrapframe * utf)
{
	cprintf("pgfault va: %08x\n", utf->utf_fault_va);
}

void
umain()
{
	set_pgfault_handler(handler);
	cprintf("%s", 0xf0000000);
}
