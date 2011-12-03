/* See COPYRIGHT for copyright information. */

#ifndef JOS_KERN_TRAP_H
#define JOS_KERN_TRAP_H
#ifndef JOS_KERNEL
# error "This is a JOS kernel header; user programs should not #include it"
#endif

#include <inc/trap.h>
#include <inc/mmu.h>

#include <inc/challenge.h>

/* The kernel's interrupt descriptor table */
extern struct Gatedesc idt[];

void idt_init(void);
void print_regs(struct PushRegs *regs);
void print_trapframe(struct Trapframe *tf);
void page_fault_handler(struct Trapframe *);
void backtrace(struct Trapframe *);

#ifdef LAB4_CHALLENGE4
void divide_by_zero_handler(struct Trapframe *);
void general_protection_fault_handler(struct Trapframe *);
void illegal_opcode_handler(struct Trapframe * tf);
#endif

#endif /* JOS_KERN_TRAP_H */
