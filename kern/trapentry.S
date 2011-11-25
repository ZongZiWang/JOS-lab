/* See COPYRIGHT for copyright information. */

#include <inc/mmu.h>
#include <inc/memlayout.h>
#include <inc/trap.h>

#include <kern/picirq.h>

#define LAB3_CHALLENGE1

###################################################################
# exceptions/interrupts
###################################################################

/* The TRAPHANDLER macro defines a globally-visible function for handling
 * a trap.  It pushes a trap number onto the stack, then jumps to _alltraps.
 * Use TRAPHANDLER for traps where the CPU automatically pushes an error code.
 */ 
#define TRAPHANDLER(name, num)						\
	.globl name;		/* define global symbol for 'name' */	\
	.type name, @function;	/* symbol type is function */		\
	.align 2;		/* align function definition */		\
	name:			/* function starts here */		\
	pushl $(num);							\
	jmp _alltraps

/* Use TRAPHANDLER_NOEC for traps where the CPU doesn't push an error code.
 * It pushes a 0 in place of the error code, so the trap frame has the same
 * format in either case.
 */
#define TRAPHANDLER_NOEC(name, num)					\
	.globl name;							\
	.type name, @function;						\
	.align 2;							\
	name:								\
	pushl $0;							\
	pushl $(num);							\
	jmp _alltraps

/*
===============================================
Macro for Challenge 1:
===============================================
*/

#define MYTRAPHANDLER(name, num, istrap, dpl)	\
	.text;										\
	.globl name;								\
	.type name, @function;						\
	.align 2;									\
	name:										\
	pushl $(num);								\
	jmp _alltraps;								\
	.data;										\
	.long name;									\
	.word istrap;								\
	.word dpl

#define MYTRAPHANDLER_NOEC(name, num, istrap, dpl)	\
	.text;										\
	.globl name;								\
	.type name, @function;						\
	.align 2;									\
	name:										\
	pushl $0;									\
	pushl $(num);								\
	jmp _alltraps;								\
	.data;										\
	.long name;									\
	.word istrap;								\
	.word dpl

#define MYTRAPHANDLER_NULL(prev, next)	\
	.data;								\
	.fill (next-prev-1), 8
	

#if defined LAB3_CHALLENGE1
/*
===============================================
After Challenge 1:
===============================================
*/
	.data;
	.globl myidt;
	.align 2;
myidt:
	MYTRAPHANDLER_NOEC(trap_divide, T_DIVIDE, 0, 0)
	MYTRAPHANDLER_NOEC(trap_debug, T_DEBUG, 1, 0)
	MYTRAPHANDLER_NOEC(trap_nmi, T_NMI, 0, 0)
	MYTRAPHANDLER_NOEC(trap_brkpt, T_BRKPT, 1, 3)
	MYTRAPHANDLER_NOEC(trap_oflow, T_OFLOW, 1, 3)
	MYTRAPHANDLER_NOEC(trap_bound, T_BOUND, 0, 3)
	MYTRAPHANDLER_NOEC(trap_illop, T_ILLOP, 0, 0)
	MYTRAPHANDLER_NOEC(trap_device, T_DEVICE, 0, 0)
	MYTRAPHANDLER(trap_dblflt, T_DBLFLT, 0, 0)
	MYTRAPHANDLER_NULL(T_DBLFLT, T_TSS)
	MYTRAPHANDLER(trap_tss, T_TSS, 0, 0)
	MYTRAPHANDLER(trap_segnp, T_SEGNP, 0, 0)
	MYTRAPHANDLER(trap_stack, T_STACK, 0, 0)
	MYTRAPHANDLER(trap_gpflt, T_GPFLT, 0, 0)
	MYTRAPHANDLER(trap_pgflt, T_PGFLT, 0, 0)
	MYTRAPHANDLER_NULL(T_PGFLT, T_FPERR)
	MYTRAPHANDLER_NOEC(trap_fperr, T_FPERR, 0, 0)
	MYTRAPHANDLER(trap_align, T_ALIGN, 0, 0)
	MYTRAPHANDLER_NOEC(trap_mchk, T_MCHK, 0, 0)
	MYTRAPHANDLER_NOEC(trap_simderr, T_SIMDERR, 0, 0)
	MYTRAPHANDLER_NULL(T_SIMDERR, T_SYSCALL)
	MYTRAPHANDLER_NOEC(trap_syscall, T_SYSCALL, 0, 3)
	MYTRAPHANDLER_NULL(T_SYSCALL, 256)

#else

/*
===============================================
Without Challeng 1:
===============================================
*/

/*
 * Lab 3: Your code here for generating entry points for the different traps.
 */

.text
	TRAPHANDLER_NOEC(trap_divide, T_DIVIDE)
	TRAPHANDLER_NOEC(trap_debug, T_DEBUG)
	TRAPHANDLER_NOEC(trap_nmi, T_NMI)
	TRAPHANDLER_NOEC(trap_brkpt, T_BRKPT)
	TRAPHANDLER_NOEC(trap_oflow, T_OFLOW)
	TRAPHANDLER_NOEC(trap_bound, T_BOUND)
	TRAPHANDLER_NOEC(trap_illop, T_ILLOP)
	TRAPHANDLER_NOEC(trap_device, T_DEVICE)
	TRAPHANDLER(trap_dblflt, T_DBLFLT)
	TRAPHANDLER(trap_tss, T_TSS)
	TRAPHANDLER(trap_segnp, T_SEGNP)
	TRAPHANDLER(trap_stack, T_STACK)
	TRAPHANDLER(trap_gpflt, T_GPFLT)
	TRAPHANDLER(trap_pgflt, T_PGFLT)
	TRAPHANDLER_NOEC(trap_fperr, T_FPERR)
	TRAPHANDLER(trap_align, T_ALIGN)
	TRAPHANDLER_NOEC(trap_mchk, T_MCHK)
	TRAPHANDLER_NOEC(trap_simderr, T_SIMDERR)	
	TRAPHANDLER_NOEC(trap_syscall, T_SYSCALL)

#endif

/*
 * Lab 3: Your code here for _alltraps
 */

_alltraps:

	pushl %ds
	pushl %es
	pusha
	pushl %esp

	movw $(GD_KD), %ax
	movw %ax, %ds
	movw %ax, %es

	call trap

/*
===============================================
The following lines are not needed because it never returns
===============================================
	
	popl %esp
	popa
	popw %es
	popw %ax
	popw %ds
	popw %ax
	iret

*/
