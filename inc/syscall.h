#ifndef JOS_INC_SYSCALL_H
#define JOS_INC_SYSCALL_H

#include <inc/challenge.h>

/* system call numbers */
enum
{
	SYS_cputs = 0,
	SYS_cgetc,
	SYS_getenvid,
	SYS_env_destroy,
	SYS_page_alloc,
	SYS_page_map,
	SYS_page_unmap,
	SYS_exofork,
	SYS_env_set_status,
	SYS_env_set_trapframe,
	SYS_env_set_pgfault_upcall,
	SYS_yield,
	SYS_ipc_try_send,
	SYS_ipc_recv,
#ifdef LAB4_CHALLENGE1
	SYS_env_set_priority,
#endif
#ifdef LAB4_CHALLENGE3
	SYS_env_restore,
#endif
#ifdef LAB4_CHALLENGE4
	SYS_env_set_gpfault_upcall,
	SYS_env_set_divide_upcall,
	SYS_env_set_illop_upcall,
#endif
#ifdef LAB4_CHALLENGE6
	SYS_page_map_update_perm,
	SYS_uxstack_pgfault_status,
#endif
	NSYSCALLS
};


#endif /* !JOS_INC_SYSCALL_H */
