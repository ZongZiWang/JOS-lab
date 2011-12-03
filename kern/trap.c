#include <inc/mmu.h>
#include <inc/x86.h>
#include <inc/assert.h>

#include <kern/pmap.h>
#include <kern/trap.h>
#include <kern/console.h>
#include <kern/monitor.h>
#include <kern/env.h>
#include <kern/syscall.h>
#include <kern/sched.h>
#include <kern/kclock.h>
#include <kern/picirq.h>

#include <inc/challenge.h>

static struct Taskstate ts;

/* Interrupt d escriptor table.  (Must be built at run time because
 * shifted function addresses can't be represented in relocation records.)
 */
struct Gatedesc idt[256] = { { 0 } };
struct Pseudodesc idt_pd = {
	sizeof(idt) - 1, (uint32_t) idt
}; 


static const char *trapname(int trapno)
{
	static const char * const excnames[] = {
		"Divide error",
		"Debug",
		"Non-Maskable Interrupt",
		"Breakpoint",
		"Overflow",
		"BOUND Range Exceeded",
		"Invalid Opcode",
		"Device Not Available",
		"Double Fault",
		"Coprocessor Segment Overrun",
		"Invalid TSS",
		"Segment Not Present",
		"Stack Fault",
		"General Protection",
		"Page Fault",
		"(unknown trap)",
		"x87 FPU Floating-Point Error",
		"Alignment Check",
		"Machine-Check",
		"SIMD Floating-Point Exception"
	};

	if (trapno < sizeof(excnames)/sizeof(excnames[0]))
		return excnames[trapno];
	if (trapno == T_SYSCALL)
		return "System call";
	if (trapno >= IRQ_OFFSET && trapno < IRQ_OFFSET + 16)
		return "Hardware Interrupt";
	return "(unknown trap)";
}


#ifdef LAB3_CHALLENGE1
//Challenge 1
extern struct Gatedesc myidt[256];
//extern struct Pseudodesc myidt_pd;
struct Pseudodesc myidt_pd = {
	sizeof(myidt) - 1, (uint32_t) myidt
};
#endif


void
idt_init(void)
{
	extern struct Segdesc gdt[];
	
	// LAB 3: Your code here.

#ifdef LAB3_CHALLENGE1
/*
===============================================
After Challenge 1:
===============================================
*/
	int i;
	for (i = 0; i < 256; i++) {
		if (*((uint32_t*)(myidt+i)) != 0) {
			SETGATE(
					idt[i],
					*((uint16_t*)(myidt+i)+2),
					GD_KT,
					*((uint32_t*)(myidt+i)),
					*((uint16_t*)(myidt+i)+3)
				   );
		}
	}

#else
/*
===============================================
Without Challenge 1:
===============================================
*/
	
	extern void trap_divide();
	extern void trap_debug();
	extern void trap_nmi();
	extern void trap_brkpt();
	extern void trap_oflow();
	extern void trap_bound();
	extern void trap_illop();
	extern void trap_device();
	extern void trap_dblflt();
	extern void trap_tss();
	extern void trap_segnp();
	extern void trap_stack();
	extern void trap_gpflt();
	extern void trap_pgflt();
	extern void trap_fperr();
	extern void trap_align();
	extern void trap_mchk();
	extern void trap_simderr();
	extern void trap_irq0();
	extern void trap_irq1();
	extern void trap_irq2();
	extern void trap_irq3();
	extern void trap_irq4();
	extern void trap_irq5();
	extern void trap_irq6();
	extern void trap_irq7();
	extern void trap_irq8();
	extern void trap_irq9();
	extern void trap_irq10();
	extern void trap_irq11();
	extern void trap_irq12();
	extern void trap_irq13();
	extern void trap_irq14();
	extern void trap_irq15();
	extern void trap_syscall();
	
	SETGATE (idt[T_DIVIDE], 0, GD_KT, trap_divide, 0);
	SETGATE (idt[T_DEBUG], 0, GD_KT, trap_debug, 0);
	SETGATE (idt[T_NMI], 0, GD_KT, trap_nmi, 0);
	SETGATE (idt[T_BRKPT], 0, GD_KT, trap_brkpt, 3);
	SETGATE (idt[T_OFLOW], 0, GD_KT, trap_oflow, 3);
	SETGATE (idt[T_BOUND], 0, GD_KT, trap_bound, 3);
	SETGATE (idt[T_ILLOP], 0, GD_KT, trap_illop, 0);
	SETGATE (idt[T_DEVICE], 0, GD_KT, trap_device, 0);
	SETGATE (idt[T_DBLFLT], 0, GD_KT, trap_dblflt, 0);
	SETGATE (idt[T_TSS], 0, GD_KT, trap_tss, 0);
	SETGATE (idt[T_SEGNP], 0, GD_KT, trap_segnp, 0);
	SETGATE (idt[T_STACK], 0, GD_KT, trap_stack, 0);
	SETGATE (idt[T_GPFLT], 0, GD_KT, trap_gpflt, 0);
	SETGATE (idt[T_PGFLT], 0, GD_KT, trap_pgflt, 0);
	SETGATE (idt[T_FPERR], 0, GD_KT, trap_fperr, 0);
	SETGATE (idt[T_ALIGN], 0, GD_KT, trap_align, 0);
	SETGATE (idt[T_MCHK], 0, GD_KT, trap_mchk, 0);
	SETGATE (idt[T_SIMDERR], 0, GD_KT, trap_simderr, 0);
	SETGATE (idt[IRQ_OFFSET+0], 0, GD_KT, trap_irq0, 0);
	SETGATE (idt[IRQ_OFFSET+1], 0, GD_KT, trap_irq1, 0);
	SETGATE (idt[IRQ_OFFSET+2], 0, GD_KT, trap_irq2, 0);
	SETGATE (idt[IRQ_OFFSET+3], 0, GD_KT, trap_irq3, 0);
	SETGATE (idt[IRQ_OFFSET+4], 0, GD_KT, trap_irq4, 0);
	SETGATE (idt[IRQ_OFFSET+5], 0, GD_KT, trap_irq5, 0);
	SETGATE (idt[IRQ_OFFSET+6], 0, GD_KT, trap_irq6, 0);
	SETGATE (idt[IRQ_OFFSET+7], 0, GD_KT, trap_irq7, 0);
	SETGATE (idt[IRQ_OFFSET+8], 0, GD_KT, trap_irq8, 0);
	SETGATE (idt[IRQ_OFFSET+9], 0, GD_KT, trap_irq9, 0);
	SETGATE (idt[IRQ_OFFSET+10], 0, GD_KT, trap_irq10, 0);
	SETGATE (idt[IRQ_OFFSET+11], 0, GD_KT, trap_irq11, 0);
	SETGATE (idt[IRQ_OFFSET+12], 0, GD_KT, trap_irq12, 0);
	SETGATE (idt[IRQ_OFFSET+13], 0, GD_KT, trap_irq13, 0);
	SETGATE (idt[IRQ_OFFSET+14], 0, GD_KT, trap_irq14, 0);
	SETGATE (idt[IRQ_OFFSET+15], 0, GD_KT, trap_irq15, 0);
	SETGATE (idt[T_SYSCALL], 0, GD_KT, trap_syscall, 3);

#endif

	// Setup a TSS so that we get the right stack
	// when we trap to the kernel.
	ts.ts_esp0 = KSTACKTOP;
	ts.ts_ss0 = GD_KD;

	// Initialize the TSS field of the gdt.
	gdt[GD_TSS >> 3] = SEG16(STS_T32A, (uint32_t) (&ts),
					sizeof(struct Taskstate), 0);
	gdt[GD_TSS >> 3].sd_s = 0;

	// Load the TSS
	ltr(GD_TSS);

	// Load the IDT
	asm volatile("lidt idt_pd");
}

void
print_trapframe(struct Trapframe *tf)
{
	cprintf("TRAP frame at %p\n", tf);
	print_regs(&tf->tf_regs);
	cprintf("  es   0x----%04x\n", tf->tf_es);
	cprintf("  ds   0x----%04x\n", tf->tf_ds);
	cprintf("  trap 0x%08x %s\n", tf->tf_trapno, trapname(tf->tf_trapno));
	cprintf("  err  0x%08x\n", tf->tf_err);
	cprintf("  eip  0x%08x\n", tf->tf_eip);
	cprintf("  cs   0x----%04x\n", tf->tf_cs);
	cprintf("  flag 0x%08x\n", tf->tf_eflags);
	cprintf("  esp  0x%08x\n", tf->tf_esp);
	cprintf("  ss   0x----%04x\n", tf->tf_ss);
}

void
print_regs(struct PushRegs *regs)
{
	cprintf("  edi  0x%08x\n", regs->reg_edi);
	cprintf("  esi  0x%08x\n", regs->reg_esi);
	cprintf("  ebp  0x%08x\n", regs->reg_ebp);
	cprintf("  oesp 0x%08x\n", regs->reg_oesp);
	cprintf("  ebx  0x%08x\n", regs->reg_ebx);
	cprintf("  edx  0x%08x\n", regs->reg_edx);
	cprintf("  ecx  0x%08x\n", regs->reg_ecx);
	cprintf("  eax  0x%08x\n", regs->reg_eax);
}

static void
trap_dispatch(struct Trapframe *tf)
{
	// Handle clock interrupts.
	// LAB 4: Your code here.

	// Handle spurious interrupts
	// The hardware sometimes raises these because of noise on the
	// IRQ line or other reasons. We don't care.
	if (tf->tf_trapno == IRQ_OFFSET + IRQ_SPURIOUS) {
		cprintf("Spurious interrupt on irq 7\n");
		print_trapframe(tf);
		return;
	}
	
	// Handle processor exceptions.
	// LAB 3: Your code here.
	
	switch (tf->tf_trapno) {
		case IRQ_OFFSET+IRQ_TIMER:
			sched_yield();
			return ;
#ifdef LAB4_CHALLENGE4
		case T_DIVIDE:
			divide_by_zero_handler(tf);
			return ;
		case T_GPFLT:
			general_protection_fault_handler(tf);
			return ;
		case T_ILLOP:
			illegal_opcode_handler(tf);
			return ;
#endif
		case T_PGFLT:
			page_fault_handler(tf);
			return ;
		case T_BRKPT:
			monitor(tf);
			return ;
		case T_SYSCALL:
			tf->tf_regs.reg_eax = syscall(
					tf->tf_regs.reg_eax,
					tf->tf_regs.reg_edx, 
					tf->tf_regs.reg_ecx,
					tf->tf_regs.reg_ebx,
					tf->tf_regs.reg_edi,
					tf->tf_regs.reg_esi);
			if (tf->tf_regs.reg_eax < 0) panic("trap dispatch: System Call fails");
			return ;
		default:

			// Unexpected trap: The user process or the kernel has a bug.
			print_trapframe(tf);
			if (tf->tf_cs == GD_KT)
				panic("unhandled trap in kernel");
			else {
				env_destroy(curenv);
				return;
			}
	}

	return ;

}

void
trap(struct Trapframe *tf)
{
	// The environment may have set DF and some versions
	// of GCC rely on DF being clear
	asm volatile("cld" ::: "cc");

	// Check that interrupts are disabled.  If this assertion
	// fails, DO NOT be tempted to fix it by inserting a "cli" in
	// the interrupt path.
	assert(!(read_eflags() & FL_IF));

	if ((tf->tf_cs & 3) == 3) {
		// Trapped from user mode.
		// Copy trap frame (which is currently on the stack)
		// into 'curenv->env_tf', so that running the environment
		// will restart at the trap point.
		assert(curenv);
		curenv->env_tf = *tf;
		// The trapframe on the stack should be ignored from here on.
		tf = &curenv->env_tf;
	}
	
	// Dispatch based on what type of trap occurred
	trap_dispatch(tf);

	// If we made it to this point, then no other environment was
	// scheduled, so we should return to the current environment
	// if doing so makes sense.
	if (curenv && curenv->env_status == ENV_RUNNABLE)
		env_run(curenv);
	else
		sched_yield();
}


void
page_fault_handler(struct Trapframe *tf)
{
	uint32_t fault_va;

	// Read processor's CR2 register to find the faulting address
	fault_va = rcr2();

	// Handle kernel-mode page faults.
	
	// LAB 3: Your code here.

	if ((tf->tf_cs & 3) == 0) panic("page fault handler: page fault in kernel-mode!");

	// We've already handled kernel-mode exceptions, so if we get here,
	// the page fault happened in user mode.

	// Call the environment's page fault upcall, if one exists.  Set up a
	// page fault stack frame on the user exception stack (below
	// UXSTACKTOP), then branch to curenv->env_pgfault_upcall.
	//
	// The page fault upcall might cause another page fault, in which case
	// we branch to the page fault upcall recursively, pushing another
	// page fault stack frame on top of the user exception stack.
	//
	// The trap handler needs one word of scratch space at the top of the
	// trap-time stack in order to return.  In the non-recursive case, we
	// don't have to worry about this because the top of the regular user
	// stack is free.  In the recursive case, this means we have to leave
	// an extra word between the current top of the exception stack and
	// the new stack frame because the exception stack _is_ the trap-time
	// stack.
	//
	// If there's no page fault upcall, the environment didn't allocate a
	// page for its exception stack or can't write to it, or the exception
	// stack overflows, then destroy the environment that caused the fault.
	// Note that the grade script assumes you will first check for the page
	// fault upcall and print the "user fault va" message below if there is
	// none.  The remaining three checks can be combined into a single test.
	//
	// Hints:
	//   user_mem_assert() and env_run() are useful here.
	//   To change what the user environment runs, modify 'curenv->env_tf'
	//   (the 'tf' variable points at 'curenv->env_tf').
	
	if (curenv->env_pgfault_upcall != NULL) {
		struct UTrapframe * utf;
		if (tf->tf_esp <= UXSTACKTOP-1 && tf->tf_esp >= UXSTACKTOP-PGSIZE)
			utf = (struct UTrapframe *)(tf->tf_esp - sizeof(struct Trapframe) - 4);
		else utf = (struct UTrapframe *)(UXSTACKTOP - sizeof(struct Trapframe));

		user_mem_assert(curenv, (void *)utf, sizeof(struct UTrapframe), (PTE_U|PTE_W));

		utf->utf_esp = tf->tf_esp;
		utf->utf_eflags = tf->tf_eflags;
		utf->utf_eip = tf->tf_eip;
		utf->utf_regs = tf->tf_regs;
		utf->utf_err = tf->tf_err;
		utf->utf_fault_va = fault_va;

		curenv->env_tf.tf_esp = (uintptr_t) utf;
		curenv->env_tf.tf_eip = (uintptr_t) curenv->env_pgfault_upcall;
		
		env_run(curenv);
	}


	// LAB 4: Your code here.

	// Destroy the environment that caused the fault.
	cprintf("[%08x] user fault va %08x ip %08x\n",
		curenv->env_id, fault_va, tf->tf_eip);
	print_trapframe(tf);
	env_destroy(curenv);
}


#ifdef LAB4_CHALLENGE4
void
divide_by_zero_handler(struct Trapframe * tf)
{

	uint32_t fault_va;
	fault_va = rcr2();

	if ((tf->tf_cs & 3) == 0) panic("divide-by-zero handler: divided by zero in kernel-mode!");
	
	if (curenv->env_divide_upcall != NULL) {
		struct UTrapframe * utf;
		if (tf->tf_esp <= UXSTACKTOP-1 && tf->tf_esp >= UXSTACKTOP-PGSIZE)
			utf = (struct UTrapframe *)(tf->tf_esp - sizeof(struct Trapframe) - 4);
		else utf = (struct UTrapframe *)(UXSTACKTOP - sizeof(struct Trapframe));

		user_mem_assert(curenv, (void *)utf, sizeof(struct UTrapframe), (PTE_U|PTE_W));

		utf->utf_esp = tf->tf_esp;
		utf->utf_eflags = tf->tf_eflags;
		utf->utf_eip = tf->tf_eip;
		utf->utf_regs = tf->tf_regs;
		utf->utf_err = tf->tf_err;
		utf->utf_fault_va = fault_va;

		curenv->env_tf.tf_esp = (uintptr_t) utf;
		curenv->env_tf.tf_eip = (uintptr_t) curenv->env_divide_upcall;
		
		env_run(curenv);
	}

	cprintf("[%08x] user divide by zero ip %08x\n",
		curenv->env_id, tf->tf_eip);
	print_trapframe(tf);
	env_destroy(curenv);

}

void
general_protection_fault_handler(struct Trapframe * tf)
{

	uint32_t fault_va;
	fault_va = rcr2();

	if ((tf->tf_cs & 3) == 0) panic("general protection fault handler: general protection fault in kernel-mode!");
	
	if (curenv->env_gpfault_upcall != NULL) {
		struct UTrapframe * utf;
		if (tf->tf_esp <= UXSTACKTOP-1 && tf->tf_esp >= UXSTACKTOP-PGSIZE)
			utf = (struct UTrapframe *)(tf->tf_esp - sizeof(struct Trapframe) - 4);
		else utf = (struct UTrapframe *)(UXSTACKTOP - sizeof(struct Trapframe));

		user_mem_assert(curenv, (void *)utf, sizeof(struct UTrapframe), (PTE_U|PTE_W));

		utf->utf_esp = tf->tf_esp;
		utf->utf_eflags = tf->tf_eflags;
		utf->utf_eip = tf->tf_eip;
		utf->utf_regs = tf->tf_regs;
		utf->utf_err = tf->tf_err;
		utf->utf_fault_va = fault_va;

		curenv->env_tf.tf_esp = (uintptr_t) utf;
		curenv->env_tf.tf_eip = (uintptr_t) curenv->env_gpfault_upcall;
		
		env_run(curenv);
	}

	cprintf("[%08x] user general protection fault ip %08x\n",
		curenv->env_id, tf->tf_eip);
	print_trapframe(tf);
	env_destroy(curenv);

}

void
illegal_opcode_handler(struct Trapframe * tf)
{

	uint32_t fault_va;
	fault_va = rcr2();

	if ((tf->tf_cs & 3) == 0) panic("illegal opcode handler: illegal opcode in kernel-mode!");
	
	if (curenv->env_illop_upcall != NULL) {
		struct UTrapframe * utf;
		if (tf->tf_esp <= UXSTACKTOP-1 && tf->tf_esp >= UXSTACKTOP-PGSIZE)
			utf = (struct UTrapframe *)(tf->tf_esp - sizeof(struct Trapframe) - 4);
		else utf = (struct UTrapframe *)(UXSTACKTOP - sizeof(struct Trapframe));

		user_mem_assert(curenv, (void *)utf, sizeof(struct UTrapframe), (PTE_U|PTE_W));

		utf->utf_esp = tf->tf_esp;
		utf->utf_eflags = tf->tf_eflags;
		utf->utf_eip = tf->tf_eip;
		utf->utf_regs = tf->tf_regs;
		utf->utf_err = tf->tf_err;
		utf->utf_fault_va = fault_va;

		curenv->env_tf.tf_esp = (uintptr_t) utf;
		curenv->env_tf.tf_eip = (uintptr_t) curenv->env_illop_upcall;
		
		env_run(curenv);
	}

	cprintf("[%08x] user illegal opcode ip %08x\n",
		curenv->env_id, tf->tf_eip);
	print_trapframe(tf);
	env_destroy(curenv);

}
#endif
