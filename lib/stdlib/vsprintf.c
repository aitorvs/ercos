#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define LEFT   1
#define SIGN   2
#define PADD   4
#define FORMAT 8
#define PLUS   16
#define LARGE  32

/*
 *
 */

const char base_s[]="0123456789abcdef";
const char base_l[]="0123456789ABCDEF";

/*
 *
 */

char *number(char *str,int number,int base,int length,int flags)
{
        unsigned int value;
        char car;
        char *tmp=str;
        char *end=str;
        char *base_n;

        value = (unsigned int)number;

        if (flags&SIGN)
        {
                if (number<0) value=(unsigned int)-number;
	}

        base_n = (char*)(flags&LARGE?base_l:base_s);

        do { *str++=base_n[value%base]; length--; } while((value=value/base));

        if (flags&FORMAT) length-=2;

        while(length-->0) *str++=flags&PADD?'0':' ';

        if (flags&FORMAT)
        {
                if (base==2)  *str++='b';
                if (base==8)  *str++='o';
                if (base==10) *str++='d';
                if (base==16) *str++='x';

		*str++='0';
        }

        if (flags&PLUS)
        {
                if (number>=0)
                {
                        *str++='+';
                }
        }

        if (flags&SIGN)
        {
                if (number<0)
                {
                        *str++='-';
                }
        }

        for (end=str--;tmp<str;tmp++,str--)
        {
                car=*tmp;*tmp=*str;*str=car;
        }

        *end=0;

        return end;
}

/*
 *
 */

int vsprintf(char *buff,const char *fmt,va_list args)
{
	char *s;
	char *str;
	int value;

	int base=10;
	int flags;
	int width=1;
	int length,i;
	

  	for (str=buff;*fmt;fmt++)
  	{
    		/* Verificamos cadena de formato "texto %a %m %[-][width]s %[width]c %[-|+|0|#][width]d|x|o|b" */

		if (*fmt!='%')
    		{
			*str++=*fmt;
			continue;
		}

		/* Procesamos flags */

		flags = 0;
		while(*fmt)
		{
			switch(*++fmt)
			{
				case '-': flags|=LEFT;   continue;
				case '+': flags|=PLUS;   continue;
				case '0': flags|=PADD;   continue;
				case '#': flags|=FORMAT; continue;
			}

			break;
		}

		/* longitud de formateo */

		width = -1;

		if (*fmt>='0'&&*fmt<='9')
		{
			width = atoi(fmt);

			while (*fmt>='0'&&*fmt<='9') fmt++;
		}

		/* Formato de salida */

		switch(*fmt)
		{
			case 's':

				s = va_arg(args,char*);

				if (!s) s="<NULL STR>";

				length = strnlen(s,width);

				if (!(flags&LEFT)) while(length<width--) *str++=' ';
				
				for (i=0;i<length;i++) *str++=*s++;

				while(length<width--) *str++=' ';

				continue;

			case 'c':

				if (!(flags&LEFT)) while(--width>0) *str++=' ';

				*str++ = (unsigned char)va_arg(args,int);

				while(--width>0) *str++=' ';

				continue;

			case 'p':

				width = 2*sizeof(void*);
				flags|= PADD;

				str = number(str,va_arg(args,int),16,width,flags);

				continue;

			case 'a':

				value = va_arg(args,unsigned int);
	
				for (i=24;i>=0;i-=8)
				{
					str = number(str,(value>>i)&0xff,10,width,0);

					if (i>0) *str++='.';
				}

				continue;
				
			case 'm':

				s = va_arg(args,char*);

				if (!s)
				{
					s = "<NULL MAC>";
					
					while(*s) *str++=*s++;
				}
				else
				{
					for (i=0;i<6;i++)
					{
						str = number(str,(unsigned char)s[i],16,2,LARGE|PADD);

						if (i<5) *str++=':';
					}
				}
				
				continue;

			case '%':
				*str++ = '%';
				continue;
			
			case 'i': 
			case 'd': flags|=SIGN;
			case 'u': base=10; break;
			case 'x': base=16; break;
			case 'X': base=16; flags|=LARGE; break;
			case 'o': base=8;  break;
			case 'b': base=2;  break;
			default: continue;
		}
	
		str = number(str,va_arg(args,int),base,width,flags);
	}

	*str=0;

	return (int)(str-buff);
}

