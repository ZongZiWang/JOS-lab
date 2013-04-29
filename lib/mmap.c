#include <inc/lib.h>
#include <inc/challenge.h>
#include <fs/fs.h>

#ifdef LAB5_CHALLENGE6

void * mmapped[32];
int modes[32];
int fds[32];
off_t offsets[32];
int mmap_cnt = 0;

#define PTE_SHARED 0x400

void *
mmap(void * start, size_t length, int prot, int mode, int fd, off_t offset)
{
	void * ret;
	int r, i, perm = PTE_P;

	if (prot & PROT_WRITE) perm |= PTE_W;
	if (prot & PROT_READ) perm |= PTE_U;
	if (mode & MAP_SHARED) perm |= PTE_SHARED;
	if (prot & PROT_NONE) perm = PTE_P;

	if ((mode & MAP_ANONYMOUS) && (fd != -1)) return MAP_FAILED;
	if ((mode & MAP_ANONYMOUS) == 0) {
		struct Fd * fd_store;
		if ((r = fd_lookup(fd, &fd_store)) < 0)
			panic("lib/mmap.c/mmap(): fd_lookup: %e", r);	
		if ((prot & PROT_READ) && fd_store->fd_omode != O_RDONLY && fd_store->fd_omode != O_RDWR) return MAP_FAILED;
		if ((prot & PROT_WRITE) && fd_store->fd_omode != O_WRONLY && fd_store->fd_omode != O_RDWR) return MAP_FAILED;
	}

	if (start != NULL) {
		ret = start;
	} else {
		uintptr_t st = UTEXT;
		int cnt = 0;
		for (; st < USTACKTOP-PGSIZE; st++) {
			if (pageref((void *)st) == 0) cnt++;
			else cnt = 0;
			if (cnt * PGSIZE >= length) break;
		}
		if (st == USTACKTOP-PGSIZE) return MAP_FAILED;
		ret = (void *)(st - (cnt-1)*PGSIZE);
	}
	
	if ((mode & MAP_ANONYMOUS) == 0) {
		seek(fd, offset);
		uintptr_t st = (uintptr_t)ret;
		for (i = 0; i < ROUNDUP(length, PGSIZE)/PGSIZE; i++) {
			if (pageref((void *)st) == 0) {
				if ((r = sys_page_alloc(0, (void *)st, perm)) < 0)
					panic("lib/mmap.c/mmap(): sys_page_alloc: %e", r);
			}
			if ((r = read(fd, (void *)st, MIN(PGSIZE, length))) < 0)
				panic("lib/mmap.c/mmap(): read: %e", r);
			st += PGSIZE;
			length -= PGSIZE;
		}
	}

	mmapped[mmap_cnt] = ret;
	modes[mmap_cnt] = mode;
	fds[mmap_cnt] = fd;
	offsets[mmap_cnt++] = offset;

	return ret;
}

int
munmap(void * start, size_t length)
{
	int r, i;
	for (i = 0; i < mmap_cnt; i++) if (mmapped[i] == start) break;

	if ((modes[i] & MAP_ANONYMOUS) == 0 && (modes[i] & MAP_PRIVATE) == 0) {
		seek(fds[i], offsets[i]);
		uintptr_t st = (uintptr_t)start;
		for (i = 0; i < ROUNDUP(length, PGSIZE)/PGSIZE; i++) {
			if ((r = write(fds[i], (void *)st, MIN(length, PGSIZE))) < 0)
				panic("lib/mmap.c/munmap(): write: %e", r);
			st += PGSIZE;
			length -= PGSIZE;
		}
	}

	for (; i < mmap_cnt-1; i++) {
		mmapped[i] = mmapped[i+1];
		modes[i] = modes[i+1];
		fds[i] = fds[i+1];
		offsets[i] = offsets[i+1];
	}
	mmap_cnt--;

	return 0;
}
#endif
