#include <string.h>

/*
 *
 */

void *memset(void *p,int c,unsigned int n)
{
	char *tmp=(char*)p;

	while(n-- > 0) *tmp++ = c;

	return p;
}

/*
 *
 */

void *memcpy(void *dest,void *src,unsigned int n)
{
	char *d=(char*)dest;
	char *s=(char*)src;
	
	while(n-->0) *d++=*s++;

	return d;
}

/*
 *
 */

int memcmp(const void *s1,const void *s2,unsigned int n)
{
	char *ps1=(char*)s1;
	char *ps2=(char*)s2;
	int res=0;

	while(n-->0) if((res=*ps1++-*ps2++)!=0) break;
	
	return res;
}

/*
 *
 */

char *strcpy(char *dest,const char *src)
{
	char *t=dest;
	char *s=(char*)src;
	
	while(*s!=0) *t++=*s++;

	*t=0;
	
	return dest;
}

/*
 *
 */

char *strncpy(char *dest,const char *src, unsigned int n)
{
	char *t=dest;
	char *s=(char*)src;
	
	while(*s!=0&&n-->0) *t++=*s++;

	*t=0;
	
	return dest;
}

/*
 *
 */

int strlen(const char *s)
{
	int c = 0;

	while(*s++) c++;

	return c;
}

/*
 *
 */

int strnlen(const char *s, unsigned int n)
{
	int c = 0;

	while(*s++&&n-->0) c++;

	return c;
}
