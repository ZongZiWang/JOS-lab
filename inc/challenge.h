#ifndef JOS_INC_CHALLENGE_H
#define JOS_INC_CHALLENGE_H

/*
 * lab1 challenge 1 [kern/console.c kern/monitor.c lib/printfmt.c]
 *	cprintf: show color
 */
#define LAB1_CHALLENGE1

/*
 * lab2 challenge 2/5 && lab3 challenge 2 [kern/monitor.c]
 *	monitor: debug command
 */
#define LAB2_CHALLENGE2
#define LAB2_CHALLENGE5
#define LAB3_CHALLENGE2

/*
 * lab3 challenge 1 [kern/trap.c kern/trapentry.S]
 *	trapentry && traphandler: lazy trap setup
 */
#define LAB3_CHALLENGE1

/*
 * the following challenges augment the system call interface:
 *
 * lab4 challenge 1 [inc/env.h inc/lib.h inc/syscall.h kern/env.c kern/sched.c kern/syscall.c lib/priority.c lib/syscall.c]
 *	priority schedule algorithm
 *
 * lab4 challenge 3 [inc/lib.h inc/syscall.h kern/syscall.c lib/syscall.c]
 *	checkpoint save and restore
 * 
 * lab4 challenge 4 [inc/env.h inc/lib.h inc/syscall.h kern/syscall.c kern/trap.c kern/trap.h lib/syscall.c lib/gpfault.c lib/divide.c lib/illop.c]
 *	more user mode trap handler
 * 
 * lab4 challenge 6 [inc/env.h inc/lib.h inc/syscall.h kern/syscall.c lib/fork.c lib/pgfault.c lib/syscall.c]
 *	faster new fork
 */
#define LAB4_CHALLENGE1
#define LAB4_CHALLENGE3
#define LAB4_CHALLENGE4
#define LAB4_CHALLENGE6

/*
 * lab4 challenge 5 [lib/fork.c]
 *	shared memory fork
 */
#define LAB4_CHALLENGE5

/*
 * lab4 challenge 7 [inc/env.h kern/env.c kern/sched.c kern/syscall.c]
 *	fairly ipc send mechanism
 */
#define LAB4_CHALLENGE7

/*
 * lab4 challenge 8 [kern/syscall.c lib/ipc.c]
 *	ipc send and receive withoud loop
 */
#define LAB4_CHALLENGE8

/*
 * lab5 challenge 6 [inc/lib.h lib/mmap.c lib/spawn.c]
 *	memory-mapped files
 */
#define LAB5_CHALLENGE6

#endif
