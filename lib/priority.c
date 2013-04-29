#include <inc/lib.h>
#include <inc/challenge.h>

#ifdef LAB4_CHALLENGE1
void
set_priority(uint32_t priority)
{
	envid_t envid = sys_getenvid();
	int r = sys_env_set_priority(envid, priority);
	if (r < 0) panic("set priority failed: %e", r);
}
#endif

