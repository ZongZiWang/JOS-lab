// User-level IPC library routines

#include <inc/lib.h>

#include <inc/challenge.h>

// Receive a value via IPC and return it.
// If 'pg' is nonnull, then any page sent by the sender will be mapped at
//	that address.
// If 'from_env_store' is nonnull, then store the IPC sender's envid in
//	*from_env_store.
// If 'perm_store' is nonnull, then store the IPC sender's page permission
//	in *perm_store (this is nonzero iff a page was successfully
//	transferred to 'pg').
// If the system call fails, then store 0 in *fromenv and *perm (if
//	they're nonnull) and return the error.
// Otherwise, return the value sent by the sender
//
// Hint:
//   Use 'env' to discover the value and who sent it.
//   If 'pg' is null, pass sys_ipc_recv a value that it will understand
//   as meaning "no page".  (Zero is not the right value, since that's
//   a perfectly valid place to map a page.)
int32_t
ipc_recv(envid_t *from_env_store, void *pg, int *perm_store)
{
	void * va = (void *)UTOP;
	if (pg != NULL) va = pg;
	
	struct Env * env = (struct Env *) envs+ENVX(sys_getenvid());

	int r = sys_ipc_recv(pg);

	if (from_env_store != NULL) {
		*from_env_store = env->env_ipc_from;
		if (r < 0) *from_env_store = 0;
	}

	if (perm_store != NULL) {
		*perm_store = env->env_ipc_perm;
		if (r < 0) *perm_store = 0;
	}

	if (r < 0) return r;
	return env->env_ipc_value;

	// LAB 4: Your code here.
	panic("ipc_recv not implemented");
	return 0;
}

// Send 'val' (and 'pg' with 'perm', if 'pg' is nonnull) to 'toenv'.
// This function keeps trying until it succeeds.
// It should panic() on any error other than -E_IPC_NOT_RECV.
//
// Hint:
//   Use sys_yield() to be CPU-friendly.
//   If 'pg' is null, pass sys_ipc_recv a value that it will understand
//   as meaning "no page".  (Zero is not the right value.)
void
ipc_send(envid_t to_env, uint32_t val, void *pg, int perm)
{
	void * va = (void *)UTOP;
	if (pg != NULL) va = pg;
	int r;
#ifdef LAB4_CHALLENGE8
	if ((r = sys_ipc_try_send(to_env, val, va, perm)) != 0) {
		if (r != -E_IPC_NOT_RECV) panic("ipc_send(): sys ipc try send failed: %e", r);
		r = sys_ipc_try_send(to_env, val, va, perm);
		if (r < 0) panic("ipc_send(): sys ipc try send failed: %e", r);
	}	
#else
	while ((r = sys_ipc_try_send(to_env, val, va, perm)) != 0) {
		if (r != -E_IPC_NOT_RECV) {
			panic("ipc_send(): sys ipc try send failed: %e", r);
		}
		sys_yield();
	}
#endif
	
	return ;
	// LAB 4: Your code here.
	panic("ipc_send not implemented");
}
