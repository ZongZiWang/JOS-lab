// implement fork from user space

#include <inc/string.h>
#include <inc/lib.h>

// PTE_COW marks copy-on-write page table entries.
// It is one of the bits explicitly allocated to user processes (PTE_AVAIL).
#define PTE_COW		0x800

extern void _pgfault_upcall(void);
//
// Custom page fault handler - if faulting page is copy-on-write,
// map in our own private writable copy.
//
static void
pgfault(struct UTrapframe *utf)
{
	void *addr = (void *) utf->utf_fault_va;
	uint32_t err = utf->utf_err;
	int r;

	// Check that the faulting access was (1) a write, and (2) to a
	// copy-on-write page.  If not, panic.
	// Hint:
	//   Use the read-only page table mappings at vpt
	//   (see <inc/memlayout.h>).

	if ((err & FEC_WR) != FEC_WR) panic("lib/fork.c/pgfault(): the faulting access was not a write!"); 
	if ((vpd[VPD(addr)] & PTE_P) != PTE_P && (vpt[VPN(addr)] & PTE_COW) != PTE_COW) panic("lib/fork.c/pgfault(): the page is not a copy-on-write page!");

	// LAB 4: Your code here.

	// Allocate a new page, map it at a temporary location (PFTEMP),
	// copy the data from the old page to the new page, then move the new
	// page to the old page's address.
	// Hint:
	//   You should make three system calls.
	//   No need to explicitly delete the old page's mapping.

	r = sys_page_alloc(0, (void *)PFTEMP, (PTE_U|PTE_P|PTE_W));
	if (r < 0) panic("lib/fork.c/pgfault(): sys_page_alloc failed. %e", r);
		
	memmove((void *)PFTEMP, ROUNDDOWN(addr, PGSIZE), PGSIZE);

	r = sys_page_map(0, (void *)PFTEMP, 0, ROUNDDOWN(addr, PGSIZE), (PTE_U|PTE_P|PTE_W));
	if (r < 0) panic("lib/fork.c/pgfault(): sys_page_map failed. %e", r);

	r = sys_page_unmap(0, (void *)PFTEMP);
	if (r < 0) panic("lib/fork.c/pgfault(): sys_page_unmap failed. %e", r);

	return ;

	// LAB 4: Your code here.

	panic("pgfault not implemented");
}

//
// Map our virtual page pn (address pn*PGSIZE) into the target envid
// at the same virtual address.  If the page is writable or copy-on-write,
// the new mapping must be created copy-on-write, and then our mapping must be
// marked copy-on-write as well.  (Exercise: Why we need to mark ours
// copy-on-write again if it was already copy-on-write at the beginning of
// this function?)
//
// Returns: 0 on success, < 0 on error.
// It is also OK to panic on error.
// 
static int
duppage(envid_t envid, unsigned pn)
{
	int r;

	void * addr = (void *) (pn * PGSIZE);

	pte_t pte = vpt[pn];

	if ((pte & PTE_P) == 0 || (pte & PTE_U) == 0) panic("lib/fork.c/duppage(): pte unpresent or unuser!");
	if ((pte & PTE_COW) == PTE_COW || (pte & PTE_W) == PTE_W) {
		r = sys_page_map(0, addr, envid, addr, PTE_COW|PTE_P|PTE_U);
		if (r < 0) panic("lib/fork.c/duppage(): COW addr %08x's page_map from envid %08x to envid %08x failed.\n", addr, sys_getenvid(), envid);
		r = sys_page_map(0, addr, 0, addr, PTE_COW|PTE_P|PTE_U);
		if (r < 0) panic("lib/fork.c/duppage(): COW addr %08x's page_map from envid %08x to envid %08x failed.\n", addr, sys_getenvid(), sys_getenvid());
	} else {
		r = sys_page_map(0, addr, envid, addr, PTE_P|PTE_U);
		if (r < 0) panic("lib/fork.c/duppage(): READONLY addr %08x's page_map from envid %08x to envid %08x failed.\n", addr, sys_getenvid(), envid);
	}

	return 0;

	// LAB 4: Your code here.
	panic("duppage not implemented");
	return 0;
}

//
// User-level fork with copy-on-write.
// Set up our page fault handler appropriately.
// Create a child.
// Copy our address space and page fault handler setup to the child.
// Then mark the child as runnable and return.
//
// Returns: child's envid to the parent, 0 to the child, < 0 on error.
// It is also OK to panic on error.
//
// Hint:
//   Use vpd, vpt, and duppage.
//   Remember to fix "env" in the child process.
//   Neither user exception stack should ever be marked copy-on-write,
//   so you must allocate a new page for the child's user exception stack.
//
envid_t
fork(void)
{
	set_pgfault_handler(pgfault);

	envid_t envid = sys_exofork();
	if (envid < 0) panic("lib/fork.c/fork(): sys exofork failed. %e", envid);

	if (envid == 0) {
		env = &envs[ENVX(sys_getenvid())];
		return 0;
	}

	uintptr_t i;

	for (i = UTEXT; i < UXSTACKTOP-PGSIZE; i += PGSIZE) {
		if ((vpd[VPD(i)] & PTE_P) && (vpt[VPN(i)] & PTE_P) && (vpt[VPN(i)] & PTE_U)) duppage(envid, VPN(i));
	}
	
	int r = sys_page_alloc(envid, (void *)(UXSTACKTOP-PGSIZE), PTE_P|PTE_U|PTE_W);
	if (r < 0) panic("lib/fork.c/fork(): sys page alloc for child UXSTACK failed: %e", r);
	sys_env_set_pgfault_upcall(envid, _pgfault_upcall);
		
	r = sys_env_set_status(envid, ENV_RUNNABLE);
	if (r < 0) panic("lib/fork.c/fork(): sys env set status failed. %e", r);

	return envid;
	
	// LAB 4: Your code here.
	panic("fork not implemented");
}

// Challenge!
int
sfork(void)
{
	panic("sfork not implemented");
	return -E_INVAL;
}
