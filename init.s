	.file	"init.c"
	.stabs	"kern/init.c",100,0,2,.Ltext0
	.text
.Ltext0:
	.stabs	"gcc2_compiled.",60,0,0,0
	.stabs	"int:t(0,1)=r(0,1);-2147483648;2147483647;",128,0,0,0
	.stabs	"char:t(0,2)=r(0,2);0;127;",128,0,0,0
	.stabs	"long int:t(0,3)=r(0,3);-2147483648;2147483647;",128,0,0,0
	.stabs	"unsigned int:t(0,4)=r(0,4);0;4294967295;",128,0,0,0
	.stabs	"long unsigned int:t(0,5)=r(0,5);0;4294967295;",128,0,0,0
	.stabs	"long long int:t(0,6)=r(0,6);-0;4294967295;",128,0,0,0
	.stabs	"long long unsigned int:t(0,7)=r(0,7);0;-1;",128,0,0,0
	.stabs	"short int:t(0,8)=r(0,8);-32768;32767;",128,0,0,0
	.stabs	"short unsigned int:t(0,9)=r(0,9);0;65535;",128,0,0,0
	.stabs	"signed char:t(0,10)=r(0,10);-128;127;",128,0,0,0
	.stabs	"unsigned char:t(0,11)=r(0,11);0;255;",128,0,0,0
	.stabs	"float:t(0,12)=r(0,1);4;0;",128,0,0,0
	.stabs	"double:t(0,13)=r(0,1);8;0;",128,0,0,0
	.stabs	"long double:t(0,14)=r(0,1);12;0;",128,0,0,0
	.stabs	"_Decimal32:t(0,15)=r(0,1);4;0;",128,0,0,0
	.stabs	"_Decimal64:t(0,16)=r(0,1);8;0;",128,0,0,0
	.stabs	"_Decimal128:t(0,17)=r(0,1);16;0;",128,0,0,0
	.stabs	"void:t(0,18)=(0,18)",128,0,0,0
	.stabs	"./inc/stdio.h",130,0,0,0
	.stabs	"./inc/stdarg.h",130,0,0,0
	.stabs	"va_list:t(2,1)=(2,2)=*(0,2)",128,0,0,0
	.stabn	162,0,0,0
	.stabn	162,0,0,0
	.stabs	"./inc/string.h",130,0,0,0
	.stabs	"./inc/types.h",130,0,0,0
	.stabs	"bool:t(4,1)=(0,1)",128,0,0,0
	.stabs	"int8_t:t(4,2)=(0,10)",128,0,0,0
	.stabs	"uint8_t:t(4,3)=(0,11)",128,0,0,0
	.stabs	"int16_t:t(4,4)=(0,8)",128,0,0,0
	.stabs	"uint16_t:t(4,5)=(0,9)",128,0,0,0
	.stabs	"int32_t:t(4,6)=(0,1)",128,0,0,0
	.stabs	"uint32_t:t(4,7)=(0,4)",128,0,0,0
	.stabs	"int64_t:t(4,8)=(0,6)",128,0,0,0
	.stabs	"uint64_t:t(4,9)=(0,7)",128,0,0,0
	.stabs	"intptr_t:t(4,10)=(4,6)",128,0,0,0
	.stabs	"uintptr_t:t(4,11)=(4,7)",128,0,0,0
	.stabs	"physaddr_t:t(4,12)=(4,7)",128,0,0,0
	.stabs	"ppn_t:t(4,13)=(4,7)",128,0,0,0
	.stabs	"size_t:t(4,14)=(4,7)",128,0,0,0
	.stabs	"ssize_t:t(4,15)=(4,6)",128,0,0,0
	.stabs	"off_t:t(4,16)=(4,6)",128,0,0,0
	.stabn	162,0,0,0
	.stabn	162,0,0,0
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"entering test_backtrace %d\n"
.LC1:
	.string	"leaving test_backtrace %d\n"
	.text
	.p2align 4,,15
	.stabs	"test_backtrace:F(0,18)",36,0,0,test_backtrace
	.stabs	"x:p(0,1)",160,0,0,8
.globl test_backtrace
	.type	test_backtrace, @function
test_backtrace:
	.stabn	68,0,13,.LM0-.LFBB1
.LM0:
.LFBB1:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%ebx
	subl	$20, %esp
	.stabn	68,0,13,.LM1-.LFBB1
.LM1:
	movl	8(%ebp), %ebx
	.stabn	68,0,14,.LM2-.LFBB1
.LM2:
	movl	$.LC0, (%esp)
	movl	%ebx, 4(%esp)
	call	cprintf
	.stabn	68,0,15,.LM3-.LFBB1
.LM3:
	testl	%ebx, %ebx
	jle	.L2
	.stabn	68,0,16,.LM4-.LFBB1
.LM4:
	leal	-1(%ebx), %eax
	movl	%eax, (%esp)
	call	test_backtrace
.L3:
	.stabn	68,0,19,.LM5-.LFBB1
.LM5:
	movl	%ebx, 4(%esp)
	movl	$.LC1, (%esp)
	call	cprintf
	.stabn	68,0,20,.LM6-.LFBB1
.LM6:
	addl	$20, %esp
	popl	%ebx
	popl	%ebp
	ret
	.p2align 4,,7
	.p2align 3
.L2:
	.stabn	68,0,18,.LM7-.LFBB1
.LM7:
	movl	$0, 8(%esp)
	movl	$0, 4(%esp)
	movl	$0, (%esp)
	call	mon_backtrace
	jmp	.L3
	.size	test_backtrace, .-test_backtrace
	.stabs	"x:r(0,1)",64,0,0,3
.Lscope1:
	.section	.rodata.str1.1
.LC2:
	.string	"-------------test-----------\n"
.LC3:
	.string	"6828 decimal is %o octal!\n"
.LC4:
	.string	"x %d, y %x, z %d\n"
.LC5:
	.string	"H%x Wo%s\n"
	.text
	.p2align 4,,15
	.stabs	"i386_init:F(0,18)",36,0,0,i386_init
.globl i386_init
	.type	i386_init, @function
i386_init:
	.stabn	68,0,24,.LM8-.LFBB2
.LM8:
.LFBB2:
	pushl	%ebp
	.stabn	68,0,30,.LM9-.LFBB2
.LM9:
	movl	$end, %eax
	.stabn	68,0,24,.LM10-.LFBB2
.LM10:
	movl	%esp, %ebp
	.stabn	68,0,30,.LM11-.LFBB2
.LM11:
	subl	$edata, %eax
	.stabn	68,0,24,.LM12-.LFBB2
.LM12:
	subl	$40, %esp
	.stabn	68,0,30,.LM13-.LFBB2
.LM13:
	movl	%eax, 8(%esp)
	movl	$0, 4(%esp)
	movl	$edata, (%esp)
	call	memset
	.stabn	68,0,34,.LM14-.LFBB2
.LM14:
	call	cons_init
	.stabn	68,0,37,.LM15-.LFBB2
.LM15:
	movl	$.LC2, (%esp)
	call	cprintf
	.stabn	68,0,39,.LM16-.LFBB2
.LM16:
	movl	$6828, 4(%esp)
	movl	$.LC3, (%esp)
	call	cprintf
	.stabn	68,0,42,.LM17-.LFBB2
.LM17:
	movl	$4, 12(%esp)
	movl	$3, 8(%esp)
	movl	$1, 4(%esp)
	movl	$.LC4, (%esp)
	call	cprintf
	.stabn	68,0,45,.LM18-.LFBB2
.LM18:
	leal	-12(%ebp), %eax
	.stabn	68,0,44,.LM19-.LFBB2
.LM19:
	movl	$6581362, -12(%ebp)
	.stabn	68,0,45,.LM20-.LFBB2
.LM20:
	movl	%eax, 8(%esp)
	movl	$57616, 4(%esp)
	movl	$.LC5, (%esp)
	call	cprintf
	.stabn	68,0,47,.LM21-.LFBB2
.LM21:
	movl	$.LC2, (%esp)
	call	cprintf
	.stabn	68,0,52,.LM22-.LFBB2
.LM22:
	movl	$5, (%esp)
	call	test_backtrace
	.p2align 4,,7
	.p2align 3
.L6:
	.stabn	68,0,56,.LM23-.LFBB2
.LM23:
	movl	$0, (%esp)
	call	monitor
	jmp	.L6
	.size	i386_init, .-i386_init
	.stabs	"i:(0,4)",128,0,0,-12
	.stabn	192,0,0,.LFBB2-.LFBB2
	.stabn	224,0,0,.Lscope2-.LFBB2
.Lscope2:
	.section	.rodata.str1.1
.LC6:
	.string	"kernel panic at %s:%d: "
.LC7:
	.string	"\n"
	.text
	.p2align 4,,15
	.stabs	"_panic:F(0,18)",36,0,0,_panic
	.stabs	"file:p(0,19)=*(0,2)",160,0,0,8
	.stabs	"line:p(0,1)",160,0,0,12
	.stabs	"fmt:p(0,19)",160,0,0,16
.globl _panic
	.type	_panic, @function
_panic:
	.stabn	68,0,72,.LM24-.LFBB3
.LM24:
.LFBB3:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	.stabn	68,0,75,.LM25-.LFBB3
.LM25:
	cmpl	$0, panicstr
	je	.L11
	.p2align 4,,7
	.p2align 3
.L10:
	.stabn	68,0,88,.LM26-.LFBB3
.LM26:
	movl	$0, (%esp)
	call	monitor
	jmp	.L10
.L11:
	.stabn	68,0,77,.LM27-.LFBB3
.LM27:
	movl	16(%ebp), %eax
	.stabn	68,0,80,.LM28-.LFBB3
.LM28:
	movl	$.LC6, (%esp)
	.stabn	68,0,77,.LM29-.LFBB3
.LM29:
	movl	%eax, panicstr
	.stabn	68,0,80,.LM30-.LFBB3
.LM30:
	movl	12(%ebp), %eax
	movl	%eax, 8(%esp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	call	cprintf
	.stabn	68,0,79,.LM31-.LFBB3
.LM31:
	leal	20(%ebp), %eax
	.stabn	68,0,81,.LM32-.LFBB3
.LM32:
	movl	%eax, 4(%esp)
	movl	16(%ebp), %eax
	movl	%eax, (%esp)
	call	vcprintf
	.stabn	68,0,82,.LM33-.LFBB3
.LM33:
	movl	$.LC7, (%esp)
	call	cprintf
	jmp	.L10
	.size	_panic, .-_panic
	.stabs	"file:r(0,19)",64,0,0,0
	.stabs	"line:r(0,1)",64,0,0,0
.Lscope3:
	.section	.rodata.str1.1
.LC8:
	.string	"kernel warning at %s:%d: "
	.text
	.p2align 4,,15
	.stabs	"_warn:F(0,18)",36,0,0,_warn
	.stabs	"file:p(0,19)",160,0,0,8
	.stabs	"line:p(0,1)",160,0,0,12
	.stabs	"fmt:p(0,19)",160,0,0,16
.globl _warn
	.type	_warn, @function
_warn:
	.stabn	68,0,94,.LM34-.LFBB4
.LM34:
.LFBB4:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$24, %esp
	.stabn	68,0,98,.LM35-.LFBB4
.LM35:
	movl	12(%ebp), %eax
	movl	$.LC8, (%esp)
	movl	%eax, 8(%esp)
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	call	cprintf
	.stabn	68,0,97,.LM36-.LFBB4
.LM36:
	leal	20(%ebp), %eax
	.stabn	68,0,99,.LM37-.LFBB4
.LM37:
	movl	%eax, 4(%esp)
	movl	16(%ebp), %eax
	movl	%eax, (%esp)
	call	vcprintf
	.stabn	68,0,100,.LM38-.LFBB4
.LM38:
	movl	$.LC7, (%esp)
	call	cprintf
	.stabn	68,0,102,.LM39-.LFBB4
.LM39:
	leave
	ret
	.size	_warn, .-_warn
	.stabs	"file:r(0,19)",64,0,0,0
	.stabs	"line:r(0,1)",64,0,0,0
.Lscope4:
	.local	panicstr
	.comm	panicstr,4,4
	.stabs	"panicstr:S(0,19)",40,0,0,panicstr
	.stabs	"",100,0,0,.Letext0
.Letext0:
	.ident	"GCC: (Ubuntu/Linaro 4.5.2-8ubuntu4) 4.5.2"
	.section	.note.GNU-stack,"",@progbits
