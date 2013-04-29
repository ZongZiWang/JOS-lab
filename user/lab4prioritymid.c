#include <inc/lib.h>
#include <inc/env.h>
#include <inc/challenge.h>

void
umain(void)
{
#ifdef LAB4_CHALLENGE1
	set_priority(PRIORITY_MIDDLE);
#endif
	int i;
	int n = 5;
	for (i = 0; i < n; i++) {
		cprintf("[%08x] Middle Priority Process is Running\n", sys_getenvid());
	}
}

