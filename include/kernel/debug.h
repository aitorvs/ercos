#ifndef DEBUG_H_
#define DEBUG_H_

#include <kernel/config.h>
#include <lib/stdio.h>

#ifdef CONFIG_DEBUG
#include <kernel/panic.h>

#define DEBUG(fmt,args...)	printf ("%s: " fmt "\n", __func__, ##args)
#define TRACE(expr,type)	DEBUG (#expr " = '%" type "'", expr)
#define ASSERT(expr)		\
	if (! (expr)) {		\
		printf (__FILE__ ":%d: %s: Assertion `" #expr "' failed.\n", __LINE__, __func__); \
		OS_KernelPanic ("Assertion `" #expr "' failed!"); \
	}

#else

#define DEBUG(fmt,args...)
#define TRACE(expr,type)
#define ASSERT(expr)		
#endif

/* debug messages even if no debug traces are requested */
#define DEBUG2(fmt,args...)	printf ("%s: " fmt "\n", __func__, ##args)

#endif /*DEBUG_H_*/
