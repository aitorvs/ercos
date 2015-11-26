void CPU_PowerDown(void);


int atoi(const char *str)
{
	int value = 0;

	while(*str>='0'&&*str<='9')
	{
		value *= 10;
		value += *str-'0';
		str++;
	}

	return value;
}

int exit(int status)
{
    CPU_PowerDown();

  /* We're not supposed to reach HERE */
 return (0);
}

