#include <kernel/debug.h>
#include <sys/types.h>

#if __GNUC__ < 3
 /* use __builtin_delete() */
#else

void operator delete(void* vp)
{
    DEBUG ("my __builtin_delete %p\n", vp);
}

/* for gcc-3.3 */

void operator delete[](void* vp){
    DEBUG ("my __builtin_delete %p\n", vp);
}

void *operator new(size_t size){
	return (void *) 0;
}

void *operator new[](size_t size){
	return (void *) 0;
}

/* __cxa_pure_virtual(void) support */

extern "C" void __cxa_pure_virtual(void) {
	DEBUG ("Attempt to use a virtual function before object has been "
			"constructed");
	while (1);
}

/* FIXME -- No idea about the next stuff */
void *__dso_handle = (void *) 0;
extern "C" int __cxa_atexit (void * (*func) (void), void *arg, void *dso_handle) {
	/* DEBUG ("(func=%p, arg=%p, dso_handle=%p); returning 0", func, arg,
			dso_handle); */
	return 0;
}


#endif
