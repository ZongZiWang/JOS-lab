#include <inc/lib.h>
#include <inc/challenge.h>

#ifdef LAB4_CHALLENGE4
extern void _divide_upcall(void);

void (*_divide_handler)(struct UTrapframe *utf);

void
set_divide_handler(void (*handler)(struct UTrapframe *utf))
{
	int r;

	if (_divide_handler == 0) {
		envid_t envid = sys_getenvid();
		int r = sys_page_alloc(envid, (void *)(UXSTACKTOP-PGSIZE), PTE_P|PTE_U|PTE_W);
		if (r < 0) panic("set divide handler: %e", r);
		sys_env_set_divide_upcall(envid, _divide_upcall);
	}

	_divide_handler = handler;
}
#endif
