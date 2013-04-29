#include <inc/lib.h>

void
umain(void)
{
	int i;
	int n = 5;
	for (i = 0; i < n; i++) {
		cprintf("[%08x] Default Priority Process is Running\n", sys_getenvid());
	}
}

