#include <inc/mmu.h>
#include <inc/memlayout.h>

.text
.globl _divide_upcall
_divide_upcall:

	pushl %esp			// function argument: pointer to UTF
	movl _divide_handler, %eax
	call *%eax
	addl $4, %esp			// pop function argument
	
	movl 40(%esp), %eax;
	add $2, %eax;//TODO How much has the %eip to be increased?
	movl 48(%esp), %ebx;
	subl $4, %ebx;
	movl %ebx, 48(%esp);
	movl %eax, (%ebx);

	addl $8, %esp;
	popal;

	addl $4, %esp;
	popfl;

	pop %esp;

	ret;
