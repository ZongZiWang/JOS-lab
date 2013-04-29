#include <inc/lib.h>
#include <inc/challenge.h>

#ifdef LAB4_CHALLENGE3
char s[256];
envid_t envid[256];
int envsaved = 0;

//TODO input file
char input[256] = "ab*cd*ef#gh#ijk**lm*t#tt#ttt##";

int
umain(void)
{
	cprintf("Please input the number of characters you will let me remember:");
	int r, c, i, j = 0, n = 0;
/*	while ((c = sys_cgetc()) != 0) {
		c = c-'0';
		n = n*10+c;
	}
*/
//TODO
	n = 19;
	cprintf("%d\n", n);

	cprintf("====================================================\n===Remember!\n===If you want to save the checkpoint, you should input '*'\n===If you want to restore the checkpoint, you should input '#'\n====================================================\n");
	for (i = 0; i < n; i++) {
		cprintf("@@@ input:");
		
//		s[i] = sys_cgetc();
//TODO
		s[i] = input[i+j];
		cprintf("%c ", s[i]);
		
		if (s[i] == '*') {
			envid[envsaved++] = env_save();
			j++;
			i--;
		} else if (s[i] == '#') {
			if (envsaved == 0) {
				cprintf("No checkpoint!");
			} else if (envid[envsaved-1] != 0) {
				if ((r = sys_env_restore(envid[envsaved-1])) < 0)
					cprintf("Checkpoint restored failed: %e", r);
			}
			envsaved--;
			j++;
			i--;
			cprintf("\n");
		} else {
			cprintf("Remembered %d chars: \"%s\"\n", i+1, s);
		}
	}
	return 0;
}
#endif
