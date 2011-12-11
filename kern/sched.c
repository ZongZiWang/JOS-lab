#include <inc/assert.h>

#include <kern/env.h>
#include <kern/pmap.h>
#include <kern/monitor.h>

#include <inc/challenge.h>

// Choose a user environment to run and run it.
void
sched_yield(void)
{
	// Implement simple round-robin scheduling.
	// Search through 'envs' for a runnable environment,
	// in circular fashion starting after the previously running env,
	// and switch to the first such environment found.
	// It's OK to choose the previously running env if no other env
	// is runnable.
	// But never choose envs[0], the idle environment,
	// unless NOTHING else is runnable.

	// LAB 4: Your code here.

	int i, curenv_idx = 0;
	if (curenv != NULL) curenv_idx = ENVX(curenv->env_id);

#ifdef LAB4_CHALLENGE1
	uint32_t max_priority = 0;
	int envidx = -1;
#endif
#ifdef LAB4_CHALLENGE7
	uint32_t min_ipc_sended = 0xffffffff;
#endif

	for (i = curenv_idx + 1; i <= curenv_idx + NENV; i++) {
		if (i % NENV != 0 && envs[i % NENV].env_status == ENV_RUNNABLE) {
#ifdef LAB4_CHALLENGE1
#ifdef LAB4_CHALLENGE7
			if ((envs[i % NENV].env_priority > max_priority)
				|| (envs[i % NENV].env_priority == max_priority && envs[i % NENV].env_ipc_sended < min_ipc_sended)) {
#else
			if (envs[i % NENV].env_priority > max_priority) {
#endif
				envidx = i % NENV;
				max_priority = envs[i % NENV].env_priority;
#ifdef LAB4_CHALLENGE7
				min_ipc_sended = envs[i % NENV].env_ipc_sended;
#endif
			}
#else
			env_run(&envs[i % NENV]);
#endif
		}
	}

#ifdef LAB4_CHALLENGE1
	if (envidx != -1) env_run(&envs[envidx]);
#endif
	
	// Run the special idle environment when nothing else is runnable.
	if (envs[0].env_status == ENV_RUNNABLE)
		env_run(&envs[0]);
	else {
		cprintf("Destroyed all environments - nothing more to do!\n");
		while (1)
			monitor(NULL);
	}
}
