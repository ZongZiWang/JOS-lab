#include <inc/lib.h>
#include <inc/challenge.h>

#ifdef LAB4_CHALLENGE4
extern void _illop_upcall(void);

void (*_illop_handler)(struct UTrapframe *utf);

void
set_illop_handler(void (*handler)(struct UTrapframe *utf))
{
	int r;

	if (_illop_handler == 0) {
		envid_t envid = sys_getenvid();
		int r = sys_page_alloc(envid, (void *)(UXSTACKTOP-PGSIZE), PTE_P|PTE_U|PTE_W);
		if (r < 0) panic("set illop handler: %e", r);
		sys_env_set_illop_upcall(envid, _illop_upcall);
	}

	_illop_handler = handler;
}
#endif
