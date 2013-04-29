#include <inc/lib.h>
#include <inc/mmu.h>
#include <lib/fork.c>
int umain()
{
	void * addr; 
	int r; 
	int offset = 0x000;

	cprintf("============ Case 1 ============\n"); 
	addr = (void *)0x1000;
	r = sys_page_alloc(0, addr, PTE_U|PTE_P|PTE_W);
	if (r < 0) panic("%e\n", r);
	cprintf("1st read: addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr, *((int *)addr));
	cprintf("2nd read: addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr, *((int *)addr));
	*((int *)addr) = 'a';
	cprintf("*((int *)%08x) = 'a'\n", addr);
	cprintf("1st write: addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr, *((int *)addr));

	cprintf("============ Case 2 ============\n"); 
	addr = (void *)(0x1000);	
	cprintf("before map: addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr+offset, *((int *)(addr+offset)));
	r = sys_page_map(0, addr, 0, addr, PTE_U|PTE_P);
	if (r < 0) panic("%e\n", r);
	cprintf("after map: addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr+offset, *((int *)(addr+offset)));
	cprintf("2nd read: addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr+offset, *((int *)(addr+offset)));
	//*((int *)(addr+offset)) = 'b';
	//cprintf("*((int *)%08x) = 'b'\n", addr+offset);
	//cprintf("addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr+offset, *((int *)(addr+offset)));
	
	cprintf("============ Case 3 ============\n"); 
	addr = (void *)(USTACKTOP-PGSIZE);	
	cprintf("before map: addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr+offset, *((int *)(addr+offset)));
	r = sys_page_map(0, addr, 0, addr, PTE_U|PTE_P);
	if (r < 0) panic("%e\n", r);
	cprintf("after map: addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr+offset, *((int *)(addr+offset)));
	cprintf("2nd read: addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr+offset, *((int *)(addr+offset)));
	*((int *)(addr+offset)) = 'c';
	cprintf("*((int *)%08x) = 'c'\n", addr+offset);
	cprintf("1st write: addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr+offset, *((int *)(addr+offset)));

	cprintf("============ Case 4 ============\n"); 
	addr = (void *)0x2000;
	r = sys_page_alloc(0, addr, PTE_U|PTE_P|PTE_COW);
	if (r < 0) panic("%e\n", r);
	cprintf("1st read: addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr, *((int *)addr));
	cprintf("2nd read: addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr, *((int *)addr));
	//*((int *)addr) = 'd';
	//cprintf("*((int *)%08x) = 'd'\n", addr);
	//cprintf("addr %08x; perm %08x; *%08x = %08x\n", addr, vpt[VPN(addr)] & 0xFFF, addr, *((int *)addr));
	
	return 0;
}
