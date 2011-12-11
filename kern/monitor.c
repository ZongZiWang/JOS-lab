// Simple command-line kernel monitor useful for
// controlling the kernel and exploring the system interactively.

#include <inc/stdio.h>
#include <inc/string.h>
#include <inc/memlayout.h>
#include <inc/assert.h>
#include <inc/x86.h>

#include <kern/console.h>
#include <kern/monitor.h>
#include <kern/kdebug.h>
#include <kern/trap.h>
#include <kern/pmap.h>
#include <inc/error.h>

#include <kern/env.h>

#include <inc/challenge.h>

#define CMDBUF_SIZE	80	// enough for one VGA text line


struct Command {
	const char *name;
	const char *desc;
	// return -1 to force monitor to exit
	int (*func)(int argc, char** argv, struct Trapframe* tf);
};

static struct Command commands[] = {
	{ "help", "Display this list of commands", mon_help },
	{ "kerninfo", "Display information about the kernel", mon_kerninfo },
	{ "backtrace", "Show the Back Trace of the current calling status", mon_backtrace },
#ifdef LAB1_CHALLENGE1
	{ "showcolor", "Display information about the color", mon_showcolor },
#endif
#ifdef LAB2_CHALLENGE5
	{ "alloc_page", "Allocate Page at certain Physical Address", mon_alloc_page },
	{ "page_status", "Display the status of Page at Given Physical Address", mon_page_status },
	{ "free_page", "Free Page at Given Physical Address", mon_free_page },
#endif
#ifdef LAB2_CHALLENGE2
	{ "showmappings", "Show the mappings from the Given Range of Virtual Addresses to the corresponding physical pages", mon_showmappings },
	{ "set_user", "Set to the User permissions of the mapping in the Given Virtual address space", mon_set_user },
	{ "clear_user", "Clear the User permissions of the mapping in the Given Virtual address space", mon_clear_user },
	{ "change_perm", "Change the User permissions of the mapping in the Given Virtual address space", mon_change_user },
	{ "set_writable", "Set to the Writable permissions of the mapping in the Given Virtual address space", mon_set_writable },
	{ "clear_writable", "Clear the Writable permissions of the mapping in the Given Virtual address space", mon_clear_writable },
	{ "change_writable", "Change the Writable permissions of the mapping in the Given Virtual address space", mon_change_writable },
	{ "set_present", "Set to the Present permissions of the mapping in the Given Virtual address space", mon_set_present },
	{ "clear_present", "Clear the Present permissions of the mapping in the Given Virtual address space", mon_clear_present },
	{ "change_present", "Change the Present permissions of the mapping in the Given Virtual address space", mon_change_present },
	{ "dump_contents_pa", "Dump the contents of a range of memory given a physical address range", mon_dump_contents_pa },
	{ "dump_contents_va", "Dump the contents of a range of memory given a virtual address range", mon_dump_contents_va },
#endif
#ifdef LAB3_CHALLENGE2
	{ "continue", "continue excution from the current location", mon_continue },
	{ "stepi", "Step one instruction exactly", mon_stepi },
#endif
};
#define NCOMMANDS (sizeof(commands)/sizeof(commands[0]))

unsigned read_eip();

/***** Implementations of basic kernel monitor commands *****/

int
mon_help(int argc, char **argv, struct Trapframe *tf)
{
	int i;

	for (i = 0; i < NCOMMANDS; i++)
		cprintf("%s - %s\n", commands[i].name, commands[i].desc);
	return 0;
}

#ifdef LAB1_CHALLENGE1
int
mon_showcolor(int argc, char **argv, struct Trapframe *tf)
{
	
	{//show letter color
		cprintf("word color:\n");
		cprintf("%Cwhitype %%Cshi before words : %CshiShine\n");
		cprintf("%Cwhitype %%Cora before words : %CoraOrange\n");
		cprintf("%Cwhitype %%Cgra before words : %CgraGray\n");
		cprintf("%Cwhitype %%Cind before words : %CindIndigo\n");
		cprintf("%Cwhitype %%Cblu before words : %CbluBlue\n");
		cprintf("%Cwhitype %%Cpur before words : %CpurPurple\n");
		cprintf("%Cwhitype %%Cgre before words : %CgreGreen\n");
		cprintf("%Cwhitype %%Cred before words : %CredRed\n");
		cprintf("%Cwhitype %%Cwhi before words : %CwhiWhite\n");
		cprintf("%Cwhitype %%Cxyz before words : color(I*8+R*4+G*2+BBshi)\n");
	}
	{//show background color
		cprintf("word background:\n");
		cprintf("%B000type %%Bshi before words : %BshiShine\n");
		cprintf("%B000type %%Bora before words : %BoraOrange\n");
		cprintf("%B000type %%Bgra before words : %BgraGray\n");
		cprintf("%B000type %%Bind before words : %BindIndigo\n");
		cprintf("%B000type %%Bblu before words : %BbluBlue\n");
		cprintf("%B000type %%Bpur before words : %BpurPurple\n");
		cprintf("%B000type %%Bgre before words : %BgreGreen\n");
		cprintf("%B000type %%Bred before words : %BredRed\n");
		cprintf("%B000type %%Bwhi before words : %Bwhi%CgraWhite\n");
		cprintf("%Cwhi%B000type %%Bxyz before words : background(I*8+R*4+G*2+BBshi)\n");
	}
	return 0;
}
#endif

#ifdef LAB2_CHALLENGE5
int isspace(int c)
{
	if (c == ' ') return 1;
	return 0;
}
long atol(const char *nptr, int base)
{
	int c;              /* current char */
	long total;         /* current total */
	int sign;           /* if '-', then negative, otherwise positive */
	/* skip whitespace */
	while ( isspace((int)(unsigned char)*nptr) )
		++nptr;
	c = (int)(unsigned char)*nptr++;
	sign = c;           /* save sign indication */
	if (c == '-' || c == '+')
		c = (int)(unsigned char)*nptr++;    /* skip sign */
	if (base == 16)	{
		c = (int)(unsigned char)*nptr++;
		c = (int)(unsigned char)*nptr++;
		//read '0x'
	}
	total = 0;
	while (!isspace(c)) {
		if ('0' <= c && c <= '9') total = base * total + (c - '0');     /* accumulate digit */
		else if ('a' <= c && c <= 'z') total = base * total + (c - 'a' + 10);
		else if ('A' <= c && c <= 'Z') total = base * total +(c - 'A' + 10);
		else break;
		c = (int)(unsigned char)*nptr++;    /* get next char */
	}
	if (sign == '-') return -total;
	else return total;   /* return result, negated if necessary */
}

int atoi(const char *nptr)
{
	return (int)atol(nptr, 10);
}
int atox(const char *nptr)
{
	return (int)atol(nptr, 16);
}

//Allocate Page
//RETURNS
//  0 -- on success
//  -E_NO_MEM -- otherwise
int mon_alloc_page(int argc, char **argv, struct Trapframe *tf)
{
	struct Page * pp;
	if (!page_alloc(&pp)) {
		pp->pp_ref++;
		cprintf("Allocating Page Succeeded! at Physical Address: 0x%x\n", page2pa(pp));
		return 0;
	}
	cprintf("Allocating Page failed!");	
	return -E_NO_MEM;
}

int mon_page_status(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 2) return -E_INVAL;
	physaddr_t pa = atox(argv[argc-1]);
	struct Page * pp = &pages[VPN(pa)];
	if (pp->pp_ref != 0) cprintf("Pages of Physical Address 0x%x is allocated!\n", pa);
	else cprintf("Pages of Physical Address 0x%x is free!\n", pa);
	return 0;
}

int mon_free_page(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 2) return -E_INVAL;
	physaddr_t pa = atox(argv[argc-1]);
	struct Page * pp = &pages[VPN(pa)];
	pp->pp_ref = 0;
	page_free(pp);
	cprintf("Freeing Pages at Physical Address 0x%x succeeded!\n", pa);
	return 0;
}
#endif

#ifdef LAB2_CHALLENGE2
int mon_showmappings(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 3) return -E_INVAL;
	uintptr_t start = atox(argv[1]);
	uintptr_t end = atox(argv[2]);
	uint32_t now = 0;
	pte_t * pte;
	for (now = ROUNDDOWN(start, PGSIZE); now <= ROUNDUP(end, PGSIZE); ) {
		struct Page * pp = page_lookup(boot_pgdir, (void *)now, &pte);
		if (pte != NULL) {
			physaddr_t pa = page2pa(pp);
			cprintf("Page %d at Virtual Address 0x%x:\n", PPN(now), now);
			cprintf("\tPhysical Address: 0x%x:\n", pa);
			cprintf("\tPermission Infomation: ");
			if (*pte & PTE_U) {
				if (*pte & PTE_W) cprintf("User RW, Sys RW\n");
				else cprintf("User R, Sys R\n");
			} else {
				if (*pte & PTE_W) cprintf("User None, Sys RW\n");
				else cprintf("User None, Sys R\n");
			}
		} else {
			cprintf("No Pages at Virtual Address 0x%x!\n", now);
		}
		now += PGSIZE;
	}
	return 0;
}

int mon_set_user(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 2) return -E_INVAL;
	uintptr_t va = atox(argv[1]);
	pte_t * pte;
	struct Page * pp = page_lookup(boot_pgdir, (void *)va, &pte);
	if (pte != NULL) {
		*pte &= PTE_U;
		cprintf("Set the User Permission Succeeded!\n");
	} else cprintf("Set the User Permission Denied!\n");
	return 0;
}
int mon_set_writable(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 2) return -E_INVAL;
	uintptr_t va = atox(argv[1]);
	pte_t * pte;
	struct Page * pp = page_lookup(boot_pgdir, (void *)va, &pte);
	if (pte != NULL) {
		*pte &= PTE_W;
		cprintf("Set the Writable Permission Succeeded!\n");
	} else cprintf("Set the Writable Permission Denied!\n");
	return 0;
}
int mon_set_present(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 2) return -E_INVAL;
	uintptr_t va = atox(argv[1]);
	pte_t * pte;
	struct Page * pp = page_lookup(boot_pgdir, (void *)va, &pte);
	if (pte != NULL) {
		*pte &= PTE_P;
		cprintf("Set the Present Permission Succeeded!\n");
	} else cprintf("Set the Present Permission Denied!\n");
	return 0;
}

int mon_clear_user(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 2) return -E_INVAL;
	uintptr_t va = atox(argv[1]);
	pte_t * pte;
	struct Page * pp = page_lookup(boot_pgdir, (void *)va, &pte);
	if (pte != NULL) {
		*pte &= ~PTE_U;
		cprintf("Clear the User Permission Succeeded!\n");
	} else cprintf("Clear the User Permission Denied!\n");
	return 0;
}
int mon_clear_writable(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 2) return -E_INVAL;
	uintptr_t va = atox(argv[1]);
	pte_t * pte;
	struct Page * pp = page_lookup(boot_pgdir, (void *)va, &pte);
	if (pte != NULL) {
		*pte &= ~PTE_W;
		cprintf("Clear the Writable Permission Succeeded!\n");
	} else cprintf("Clear the Writable Permission Denied!\n");
	return 0;
}
int mon_clear_present(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 2) return -E_INVAL;
	uintptr_t va = atox(argv[1]);
	pte_t * pte;
	struct Page * pp = page_lookup(boot_pgdir, (void *)va, &pte);
	if (pte != NULL) {
		*pte &= ~PTE_P;
		cprintf("Clear the Present Permission Succeeded!\n");
	} else cprintf("Clear the Present Permission Denied!\n");
	return 0;
}
int mon_change_user(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 2) return -E_INVAL;
	uintptr_t va = atox(argv[1]);
	pte_t * pte;
	struct Page * pp = page_lookup(boot_pgdir, (void *)va, &pte);
	if (pte != NULL) {
		*pte ^= PTE_U;
		cprintf("Change the User Permission Succeeded!\n");
	} else cprintf("Change the User Permission Denied!\n");
	return 0;
}
int mon_change_writable(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 2) return -E_INVAL;
	uintptr_t va = atox(argv[1]);
	pte_t * pte;
	struct Page * pp = page_lookup(boot_pgdir, (void *)va, &pte);
	if (pte != NULL) {
		*pte ^= PTE_W;
		cprintf("Change the Writable Permission Succeeded!\n");
	} else cprintf("Change the Writable Permission Denied!\n");
	return 0;
}
int mon_change_present(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 2) return -E_INVAL;
	uintptr_t va = atox(argv[1]);
	pte_t * pte;
	struct Page * pp = page_lookup(boot_pgdir, (void *)va, &pte);
	if (pte != NULL) {
		*pte ^= PTE_U;
		cprintf("Change the Present Permission Succeeded!\n");
	} else cprintf("Change the Present Permission Denied!\n");
	return 0;
}
int mon_dump_contents_pa(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 3) return -E_INVAL;
	physaddr_t start = atox(argv[1]);
	physaddr_t end = atox(argv[2]);
	physaddr_t now = 0;
	cprintf("%08x: ", now);
	for (now = start; now <= end; now++) {
		if (now % 4 == 0) cprintf("\n%08x: ", now);
		cprintf("0x%08x ", *((uint32_t*)KADDR(now)));
	}
	cprintf("\n");
			
	return 0;
}
int mon_dump_contents_va(int argc, char **argv, struct Trapframe *tf)
{
	if (argc != 3 && argc != 4) return -E_INVAL;
	uintptr_t start;
	uintptr_t end;
	pde_t * pgdir = boot_pgdir;
	if (argc == 4) {
		uintptr_t envid = atox(argv[1]);
		struct Env * env;
		envid2env(envid, &env, 0);
		start = atox(argv[2]);
		end = atox(argv[3]);
		pgdir = env->env_pgdir;
	} else {
		start = atox(argv[1]);
		end = atox(argv[2]);
	}
	uintptr_t now = 0, next;
	pte_t * pte;
	for (now = start; now <= end; ) {
		struct Page * pp = page_lookup(pgdir, (void *)now, &pte);
		if (pte != NULL) {
			physaddr_t pa = page2pa(pp);
			cprintf("Page %d at Virtual Address 0x%x:\n", PPN(now), now);
			cprintf("%08x: ", now);
			for (next = now; next <= end && next < ROUNDUP(now+1, PGSIZE); next++) {
				if (next % 4 == 0) cprintf("\n%08x: ", next);
				cprintf("0x%08x ", *((uint32_t*)next));
			}
			cprintf("\n");
		} else {
			cprintf("No Pages at Virtual Address 0x%x!\n", now);
			next = ROUNDUP(now+1, PGSIZE);
		}
		now = next;
	}
	return 0;
}
#endif

#ifdef LAB3_CHALLENGE2
int mon_continue(int argc, char **argv, struct Trapframe *tf)
{
	if (tf == NULL) {
		cprintf("Can not continue!\n");
		return -1;
	}
	cprintf("continuing!\n");
	uint32_t eflags = tf->tf_eflags;
	eflags |= FL_RF;
	eflags &= ~FL_TF;
	tf->tf_eflags = eflags;
	
	return 0;
}
int mon_stepi(int argc, char **argv, struct Trapframe *tf)
{
	if (tf == NULL) {
		cprintf("Can not step instruction!\n");
		return -1;
	}
	cprintf("step!\n");
	uint32_t eflags = tf->tf_eflags;
	eflags |= FL_TF;
	tf->tf_eflags = eflags;
	
	return 0;
}
#endif

int	
mon_kerninfo(int argc, char **argv, struct Trapframe *tf)
{
	extern char _start[], etext[], edata[], end[];

	cprintf("Special kernel symbols:\n");
	cprintf("  _start %08x (virt)  %08x (phys)\n", _start, _start - KERNBASE);
	cprintf("  etext  %08x (virt)  %08x (phys)\n", etext, etext - KERNBASE);
	cprintf("  edata  %08x (virt)  %08x (phys)\n", edata, edata - KERNBASE);
	cprintf("  end    %08x (virt)  %08x (phys)\n", end, end - KERNBASE);
	cprintf("Kernel executable memory footprint: %dKB\n",
		(end-_start+1023)/1024);
	return 0;
}

int
mon_backtrace(int argc, char **argv, struct Trapframe *tf)
{
	//ebp eip arg[0..4] represent what we should show
	uint32_t *ebp, eip, arg[5];
	struct Eipdebuginfo info;
	int i;
	cprintf ("Stack backtrace:\n");
	//get current ebp address by read_ebp() 
	for (ebp = (uint32_t*) read_ebp(); ebp != NULL; ebp = (uint32_t*) ebp[0])
	{
		//eip's address ebp+1
		eip = ebp[1];
		//arg[i]'s address ebp+i+2
		for (i = 0; i < 5; i++) arg[i] = ebp[i+2];

		cprintf("  ebp %08x eip %08x args %08x %08x %08x %08x %08x\n", ebp, eip, arg[0], arg[1], arg[2], arg[3], arg[4]);
		int result = debuginfo_eip(eip, &info);
		cprintf("         %s:%d: %.*s+%d\n", info.eip_file, info.eip_line, info.eip_fn_namelen, info.eip_fn_name, eip-info.eip_fn_addr);

	}

	// Your code here.
	return 0;
}



/***** Kernel monitor command interpreter *****/

#define WHITESPACE "\t\r\n "
#define MAXARGS 16

static int
runcmd(char *buf, struct Trapframe *tf)
{
	int argc;
	char *argv[MAXARGS];
	int i;

	// Parse the command buffer into whitespace-separated arguments
	argc = 0;
	argv[argc] = 0;
	while (1) {
		// gobble whitespace
		while (*buf && strchr(WHITESPACE, *buf))
			*buf++ = 0;
		if (*buf == 0)
			break;

		// save and scan past next arg
		if (argc == MAXARGS-1) {
			cprintf("Too many arguments (max %d)\n", MAXARGS);
			return 0;
		}
		argv[argc++] = buf;
		while (*buf && !strchr(WHITESPACE, *buf))
			buf++;
	}
	argv[argc] = 0;

	// Lookup and invoke the command
	if (argc == 0)
		return 0;
	for (i = 0; i < NCOMMANDS; i++) {
		if (strcmp(argv[0], commands[i].name) == 0)
			return commands[i].func(argc, argv, tf);
	}
	cprintf("Unknown command '%s'\n", argv[0]);
	return 0;
}

void
monitor(struct Trapframe *tf)
{
	char *buf;

	cprintf("Welcome to the JOS kernel monitor!\n");
	cprintf("Type 'help' for a list of commands.\n");

	if (tf != NULL)
		print_trapframe(tf);

	while (1) {
		buf = readline("K> ");
		if (buf != NULL)
			if (runcmd(buf, tf) < 0)
				break;
	}
}

// return EIP of caller.
// does not work if inlined.
// putting at the end of the file seems to prevent inlining.
unsigned
read_eip()
{
	uint32_t callerpc;
	__asm __volatile("movl 4(%%ebp), %0" : "=r" (callerpc));
	return callerpc;
}
