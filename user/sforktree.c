// Fork a binary tree of processes and display their structure.

#include <inc/lib.h>
#include <inc/env.h>
#include <inc/challenge.h>

#define DEPTH 3

void forktree(const char *cur);

int cnt = 0;
#ifdef LAB4_CHALLENGE1
int priority = PRIORITY_LOW;
#endif

void
forkchild(const char *cur, char branch)
{
	char nxt[DEPTH+1];

	if (strlen(cur) >= DEPTH)
		return;

	snprintf(nxt, DEPTH+1, "%s%c", cur, branch);

	if (sfork() == 0) {
		forktree(nxt);
		exit();
	}
}

void
forktree(const char *cur)
{
#ifdef LAB4_CHALLENGE1
	set_priority(priority+cnt);
#endif
	cnt++;
	cprintf("%04x: I am '%s', the %dth environment\n", sys_getenvid(), cur, cnt);
	int tmp = cnt;

	forkchild(cur, '0');
	sys_yield();
	forkchild(cur, '1');
	sys_yield();
	
	cprintf("%04x: I am '%s', have %d offspring\n", sys_getenvid(), cur, cnt-tmp);
		
}

void
umain(void)
{
	forktree("");
}

