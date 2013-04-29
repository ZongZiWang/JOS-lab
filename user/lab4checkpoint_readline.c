#include <inc/lib.h>
#include <inc/challenge.h>

#ifdef LAB4_CHALLENGE3

#define N 1024
int a[N];
envid_t envid[NENV];
int envsaved = 0;

int isspace(int c)
{
	if (c == ' ') return 1;
	return 0;
}
long atol(const char *nptr, int base)
{
	int c;              /* current char */
	long total;         /* current total */
	int sign;           /* if '-', then negative, otherwise positive */
	/* skip whitespace */
	while ( isspace((int)(unsigned char)*nptr) )
		++nptr;
	c = (int)(unsigned char)*nptr++;
	sign = c;           /* save sign indication */
	if (c == '-' || c == '+')
		c = (int)(unsigned char)*nptr++;    /* skip sign */
	if (base == 16)	{
		c = (int)(unsigned char)*nptr++;
		c = (int)(unsigned char)*nptr++;
		//read '0x'
	}
	total = 0;
	while (!isspace(c)) {
		if ('0' <= c && c <= '9') total = base * total + (c - '0');     /* accumulate digit */
		else if ('a' <= c && c <= 'z') total = base * total + (c - 'a' + 10);
		else if ('A' <= c && c <= 'Z') total = base * total +(c - 'A' + 10);
		else break;
		c = (int)(unsigned char)*nptr++;    /* get next char */
	}
	if (sign == '-') return -total;
	else return total;   /* return result, negated if necessary */
}
int atoi(const char *nptr)
{
	return (int)atol(nptr, 10);
}

int
umain(void)
{
	char * input;
	int i, j, n = 0;
	input = readline("Please input the number of numbers you will let me remember:");
	n = atoi(input);
	cprintf("###Remember!\n###If you want to save the checkpoint, you should input '*'\n###If you want to restore the checkpoint, you should input '#'\n");
	for (i = 0; i < n; i++) {
		input = readline("Please input the number or '*' or '#' you want me to remember:");
		if (input[0] == '*') {
			envid[envsaved++] = env_save();
			i--;
		} else if (input[0] == '#') {
			env_restore(envid[envsaved-1]);
			i--;
		} else {
			a[i] = atoi(input);
			cprintf("I have remembered %d numbers! They are {%d", a[0]);
			for (j = 1; j <= i; j++) cprintf(", %d", a[j]);
			cprintf("}\n");
		}
	}
	return 0;
}
#endif
