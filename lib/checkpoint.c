#include <inc/lib.h>
#include <inc/env.h>

#include <inc/challenge.h>

envid_t
env_save(void)
{
	int envid;
	if ((envid = fork()) == 0) {
		sys_env_set_status(0, ENV_NOT_RUNNABLE);
		sys_yield();
	} else sys_yield();
	return envid;	
}

