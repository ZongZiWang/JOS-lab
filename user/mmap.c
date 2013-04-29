#include <inc/lib.h>
#include <inc/challenge.h>

void
umain(void)
{
#ifdef LAB5_CHALLENGE6
	int fd, n, r;
	char * s;
	char rdbuf[512+1];
	char wrbuf[512+1] = "memory-mapped file!\n";

	cprintf("mmap: open /mmapfile\n");
	if ((fd = open("/mmapfile", O_RDWR|O_CREAT)) < 0)
		panic("mmap: open /mmapfile: %e", fd);

	cprintf("mmap: read /mmapfile\n");
	if ((r = read(fd, rdbuf, sizeof(rdbuf))) < 0)
		panic("mmap: read /mmapfile: %e", fd);

	cprintf("MMAPFILE before mmap()\n===\n%s===\n", rdbuf);

	cprintf("[%08x] mmap: mmap /mmapfile\n", sys_getenvid());
	if ((s = (char *)mmap(NULL, sizeof(wrbuf), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
		panic("mmap: mmap /mmapfile: %e", -E_INVAL);

	cprintf("MEMORY-mmaped MMAPFILE after mmap()\n===\n%s===\n", s);
	
	cprintf("[%08x] mmap: strcpy(mmap'return_address, write_buffer)\n", sys_getenvid(), wrbuf);	
	strcpy(s, wrbuf);
	
	cprintf("MEMORY-mmaped MMAPFILE after srtcpy()\n===\n%s===\n", s);

	cprintf("mmap: munmap /mmapfile\n");
	if ((r = munmap(s, sizeof(wrbuf))) < 0)
		panic("mmap: munmap /mmapfile: %e", -E_INVAL);

	cprintf("mmap: close /mmapfile\n");
	if ((r = close(fd)) < 0)
		panic("mmap: close /mmapfile: %e", r);

	cprintf("mmap: open /mmapfile\n");
	if ((fd = open("/mmapfile", O_RDONLY)) < 0)
		panic("mmap: open /mmapfile: %e", fd);

	cprintf("mmap: read /mmapfile\n");
	if ((r = read(fd, rdbuf, sizeof(rdbuf))) < 0)
		panic("mmap: read /mmapfile: %e", fd);

	cprintf("MMAPFILE after mmap()\n===\n%s===\n", rdbuf);

	cprintf("mmap: close /mmapfile\n");
	if ((r = close(fd)) < 0)
		panic("mmap: close /mmapfile: %e", r);
#endif
}
