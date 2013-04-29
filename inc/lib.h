// Main public header file for our user-land support library,
// whose code lives in the lib directory.
// This library is roughly our OS's version of a standard C library,
// and is intended to be linked into all user-mode applications
// (NOT the kernel or boot loader).

#ifndef JOS_INC_LIB_H
#define JOS_INC_LIB_H 1

#include <inc/types.h>
#include <inc/stdio.h>
#include <inc/stdarg.h>
#include <inc/string.h>
#include <inc/error.h>
#include <inc/assert.h>
#include <inc/env.h>
#include <inc/memlayout.h>
#include <inc/syscall.h>
#include <inc/trap.h>
#include <inc/fs.h>
#include <inc/fd.h>
#include <inc/args.h>

#define USED(x)		(void)(x)

#include <inc/challenge.h>

// libos.c or entry.S
extern char *binaryname;
extern volatile struct Env *env;
extern volatile struct Env envs[NENV];
extern volatile struct Page pages[];
void	exit(void);

// pgfault.c
void	set_pgfault_handler(void (*handler)(struct UTrapframe *utf));

#ifdef LAB4_CHALLENGE1
// priority.c
void set_priority(uint32_t priority);
#endif
#ifdef LAB4_CHALLENGE3
// checkpoint.c
envid_t env_save(void);
#endif
#ifdef LAB4_CHALLENGE4
// gpfault.c
void	set_gpfault_handler(void (*handler)(struct UTrapframe *utf));
// divide.c
void	set_divide_handler(void (*handler)(struct UTrapframe *utf));
// illop.c
void	set_illop_handler(void (*handler)(struct UTrapframe *utf));
#endif

// readline.c
char*	readline(const char *buf);

// syscall.c
void	sys_cputs(const char *string, size_t len);
int	sys_cgetc(void);
envid_t	sys_getenvid(void);
int	sys_env_destroy(envid_t);
void	sys_yield(void);
static envid_t sys_exofork(void);
int	sys_env_set_status(envid_t env, int status);
int	sys_env_set_trapframe(envid_t env, struct Trapframe *tf);
int	sys_env_set_pgfault_upcall(envid_t env, void *upcall);
#ifdef LAB4_CHALLENGE1
int sys_env_set_priority(envid_t env, uint32_t priority);
#endif
#ifdef LAB4_CHALLENGE3
int sys_env_restore(envid_t env);
#endif
#ifdef LAB4_CHALLENGE4
int	sys_env_set_gpfault_upcall(envid_t env, void *upcall);
int	sys_env_set_divide_upcall(envid_t env, void *upcall);
int	sys_env_set_illop_upcall(envid_t env, void *upcall);
#endif
#ifdef LAB4_CHALLENGE6
int sys_page_map_update_perm(envid_t src_env, void *src, envid_t dst_env, void *dst_pg, int perm);
int sys_uxstack_pgfault_status(envid_t envid, void *pg, int perm, void *upcall, int status); 
#endif
int	sys_page_alloc(envid_t env, void *pg, int perm);
int	sys_page_map(envid_t src_env, void *src_pg,
		     envid_t dst_env, void *dst_pg, int perm);
int	sys_page_unmap(envid_t env, void *pg);
int	sys_ipc_try_send(envid_t to_env, uint32_t value, void *pg, int perm);
int	sys_ipc_recv(void *rcv_pg);

// This must be inlined.  Exercise for reader: why?
static __inline envid_t sys_exofork(void) __attribute__((always_inline));
static __inline envid_t
sys_exofork(void)
{
	envid_t ret;
	__asm __volatile("int %2"
		: "=a" (ret)
		: "a" (SYS_exofork),
		  "i" (T_SYSCALL)
	);
	return ret;
}

// ipc.c
void	ipc_send(envid_t to_env, uint32_t value, void *pg, int perm);
int32_t ipc_recv(envid_t *from_env_store, void *pg, int *perm_store);

// fork.c
#define	PTE_SHARE	0x400
envid_t	fork(void);
envid_t	sfork(void);	// Challenge!

// fd.c
int	close(int fd);
ssize_t	read(int fd, void *buf, size_t nbytes);
ssize_t	write(int fd, const void *buf, size_t nbytes);
int	seek(int fd, off_t offset);
void	close_all(void);
ssize_t	readn(int fd, void *buf, size_t nbytes);
int	dup(int oldfd, int newfd);
int	fstat(int fd, struct Stat *statbuf);
int	stat(const char *path, struct Stat *statbuf);

// file.c
int	open(const char *path, int mode);
int	ftruncate(int fd, off_t size);
int	remove(const char *path);
int	sync(void);

// pageref.c
int	pageref(void *addr);


// spawn.c
envid_t	spawn(const char *program, const char **argv);
envid_t	spawnl(const char *program, const char *arg0, ...);


/* File open modes */
#define	O_RDONLY	0x0000		/* open for reading only */
#define	O_WRONLY	0x0001		/* open for writing only */
#define	O_RDWR		0x0002		/* open for reading and writing */
#define	O_ACCMODE	0x0003		/* mask for above modes */

#define	O_CREAT		0x0100		/* create if nonexistent */
#define	O_TRUNC		0x0200		/* truncate to zero length */
#define	O_EXCL		0x0400		/* error if already exists */
#define O_MKDIR		0x0800		/* create directory, not regular file */

#ifdef LAB5_CHALLENGE6

// mmap.c
void * mmap(void * start, size_t length, int prot, int mode, int fd, off_t offset);
int munmap(void * start, size_t length);

/* File map modes */
#define PROT_EXEC 0x0001		//页内容可以被执行
#define PROT_READ 0x0002		//页内容可以被读取
#define PROT_WRITE 0x0004		//页可以被写入
#define PROT_NONE 0x0008		//页不可访问
#define MAP_FIXED 0x0001		//使用指定的映射起始地址，如果由start和len参数指定的内存区重叠于现存的映射空间，重叠部分将会被丢弃。如果指定的起始地址不可用，操作将会失败。并且起始地址必须落在页的边界上。
#define MAP_SHARED 0x0002		//与其它所有映射这个对象的进程共享映射空间。对共享区的写入，相当于输出到文件。直到msync()或者munmap()被调用，文件实际上不会被更新。
#define MAP_PRIVATE 0x0004		//建立一个写入时拷贝的私有映射。内存区域的写入不会影响到原文件。这个标志和以上标志是互斥的，只能使用其中一个。
#define MAP_DENYWRITE 0x0008	//这个标志被忽略。
#define MAP_EXECUTABLE 0x0010	//同上
#define MAP_NORESERVE 0x0020	//不要为这个映射保留交换空间。当交换空间被保留，对映射区修改的可能会得到保证。当交换空间不被保留，同时内存不足，对映射区的修改会引起段违例信号。
#define MAP_LOCKED 0x0040		//锁定映射区的页面，从而防止页面被交换出内存。
#define MAP_GROWSDOWN 0x0080	//用于堆栈，告诉内核VM系统，映射区可以向下扩展。
#define MAP_ANONYMOUS 0x0100	//匿名映射，映射区不与任何文件关联。
#define MAP_ANON 0x0200			//MAP_ANONYMOUS的别称，不再被使用。
#define MAP_FILE 0x0400			//兼容标志，被忽略。
#define MAP_32BIT 0x0800		//将映射区放在进程地址空间的低2GB，MAP_FIXED指定时会被忽略。当前这个标志只在x86-64平台上得到支持。
#define MAP_POPULATE 0x1000		//为文件映射通过预读的方式准备好页表。随后对映射区的访问不会被页违例阻塞。
#define MAP_NONBLOCK 0x2000		//仅和MAP_POPULATE一起使用时才有意义。不执行预读，只为已存在于内存中的页面建立页表入口。

#define MAP_FAILED ((void *)-1)
#endif


#endif	// !JOS_INC_LIB_H
