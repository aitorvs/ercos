
/*
    Copyright 2004-2005 (c) by Cesar Rodriguez Ortega
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*
 * Miscellaneous test -- See code annotations for more help
 */

#include <stdio.h>
#include <posix/time.h>
#include <kernel/debug.h>

#include "global_test.h"
//#include "edroombp.h"

DEFINE_TEST(10);

#define DEBUG_TSPEC(msg,ts) DEBUG2 (msg "tv_sec=%u, tv_nsec=%u", (ts)->tv_sec, (ts)->tv_nsec)

#define PRINT(task,fmt,args...) \
	clock_gettime (CLOCK_REALTIME, &tp); \
	printf ("%03d,%03ds: T%d: " fmt "\n", tp.tv_sec, tp.tv_nsec / 1000000, \
			task, ##args);

/* ** internal variables ** */
static unsigned long _clock_gettime_latency = 0;
/* ** end of internal variables ** */

void clock_gettime_lat (void) {
	register unsigned i;
	register unsigned long us;
	struct timespec in, fi100, fi1000, fi5000;

	printf ("\n ** clock_gettime syscall latency test...\n");

	/* get initial time */
	clock_gettime (CLOCK_REALTIME, &in);

	/* get the time 100 times */
	for (i = 0; i < 25; i++) {
		clock_gettime (CLOCK_REALTIME, &fi100);
		clock_gettime (CLOCK_REALTIME, &fi100);
		clock_gettime (CLOCK_REALTIME, &fi100);
		clock_gettime (CLOCK_REALTIME, &fi100);
	}

	/* get the time 1000 - 100 = 900 times more */
	for (i = 0; i < 225; i++) {
		clock_gettime (CLOCK_REALTIME, &fi1000);
		clock_gettime (CLOCK_REALTIME, &fi1000);
		clock_gettime (CLOCK_REALTIME, &fi1000);
		clock_gettime (CLOCK_REALTIME, &fi1000);
	}

	/* get the time 5000 - 1000 = 4000 times more */
	for (i = 0; i < 1000; i++) {
		clock_gettime (CLOCK_REALTIME, &fi5000);
		clock_gettime (CLOCK_REALTIME, &fi5000);
		clock_gettime (CLOCK_REALTIME, &fi5000);
		clock_gettime (CLOCK_REALTIME, &fi5000);
	}

	/* clock_gettime latency for the 100 iteration test */
	us = (fi100.tv_sec - in.tv_sec) * 1000000;
	us += (fi100.tv_nsec - in.tv_nsec) / 1000;
	printf ("    latency (x100 test) is  %uus\n", us / 100);

	/* clock_gettime latency for the 1000 iteration test */
	us = (fi1000.tv_sec - in.tv_sec) * 1000000;
	us += (fi1000.tv_nsec - in.tv_nsec) / 1000;
	printf ("    latency (x1000 test) is %uus\n", us / 1000);

	/* clock_gettime latency for the 5000 iteration test */
	us = (fi5000.tv_sec - in.tv_sec) * 1000000;
	us += (fi5000.tv_nsec - in.tv_nsec) / 1000;
	_clock_gettime_latency = us / 5000;
	printf ("    latency (x5000 test) is %uus\n", _clock_gettime_latency);

	printf ("    > Test Result: mean latency seems to be %uus\n", _clock_gettime_latency);

	printf (" ** clock_gettime syscall latency test... done.\n");
}

void _sleep_accuracy (const char * msg, unsigned long us) {
	struct timespec in, fi, nsl;
	register unsigned i, times;
	long long dns;
	long dev, percent;

	/* the test gets repeated a minimum of 5 times or 10 seconds of
	 * execution time, whichever is high */
	times = 10000000 / us;
	if (times < 5) times = 5;

	/* and a number of times multiple of 5 */
	times /= 5;
	times *= 5;

	printf ("\n   + %dus [%s] test (x%d test)...\n", us, msg, times);

	/* initialize the time to sleep */
	nsl.tv_sec = us / 1000000;
	nsl.tv_nsec = (us % 1000000) * 1000;

	/* the loop is unrolled 5 times */
	times /= 5;

	/* get initial time */
	clock_gettime (CLOCK_REALTIME, &in);

	for (i = 0; i < times; i++) {
		nanosleep (&nsl, (struct timespec *) 0);
		nanosleep (&nsl, (struct timespec *) 0);
		nanosleep (&nsl, (struct timespec *) 0);
		nanosleep (&nsl, (struct timespec *) 0);
		nanosleep (&nsl, (struct timespec *) 0);
	}

	/* get final time */
	clock_gettime (CLOCK_REALTIME, &fi);

	/* times was times*5 ! */
	times *= 5;

	/* check delayed time (in ns) */
	dns = (fi.tv_sec - in.tv_sec) * 1000000000ull;
	dns += ((long) fi.tv_nsec - (long) in.tv_nsec);

	printf ("     Total requested time is %dus; elapsed %dus\n",
			us * times, (long) (dns / 1000ll));

	/* mean slept time */
	dns /= (long) times;
	printf ("     Mean slept time is %dus\n", (long) (dns / 1000ll));

	/* mean deviation time */
	dev = dns - us * 1000ul;
	percent = dev / ((long) us * 10l);
	printf ("     > Test Result: mean deviation in %d tests seems to be "
			"%+dns, \n"
		"       which is %+d%% of the requested time\n",
			times, dev, percent);

	printf ("   + %dus [%s] test (x%d test)... done.\n", us, msg, times);
}

void sleep_accuracy (void) {
	
	printf ("\n ** Sleep accuracy test... (remember the %dus clock_gettime "
			"latency!)\n", _clock_gettime_latency);

	/* _sleep_accuracy ("5ms", 5000);
	_sleep_accuracy ("10ms", 10000);
	_sleep_accuracy ("15ms", 15000);
	_sleep_accuracy ("20ms", 20000);
	_sleep_accuracy ("25ms", 25000);
	_sleep_accuracy ("30ms", 30000);
	_sleep_accuracy ("35ms", 35000);
	_sleep_accuracy ("40ms", 40000);
	_sleep_accuracy ("50ms", 50000); */
	_sleep_accuracy ("70ms", 70000);
	_sleep_accuracy ("100ms", 100000);
	_sleep_accuracy ("300ms", 300000);
	_sleep_accuracy ("1s", 1000000);
	_sleep_accuracy ("5s", 5000000);

	printf (" ** Sleep accuracy test... done.\n");
}

void task_sleep (void) {
	/* in future releases :) */
}

long long _loop_lat (register long iters) {

	struct timespec in, fi;
	long long ret;

	printf ("   + Testing with %d iterations... ", iters);

	clock_gettime (CLOCK_REALTIME, &in);
	for (volatile long i = 0; i < iters; i++);
	clock_gettime (CLOCK_REALTIME, &fi);

	ret = (fi.tv_sec - in.tv_sec) * 1000000000ll + fi.tv_nsec - in.tv_nsec;
	printf ("%dus\n", (long) (ret / 1000ll));
	return ret;
}

static long long llabs (long long val) { return val < 0 ? -val : val; }

void loop_lat (long estimated, long long desired_delay, long long threshold) {

	float factor;
	long long delay;
	long iters, test;

	printf ("\n ** Loop latency test (desired delay of %dms...)\n",
			(long) (desired_delay / 1000000ll));

	iters = estimated;
	for (test = 1; test <= 10; test++) {
		/* perform new try */
		delay = _loop_lat (iters);

		/* got an acceptable delay ? */
		if (llabs (desired_delay - delay) < threshold) break;

		/* calculate a factor to modify the iterations */
		factor = desired_delay / (float) delay;

		/* modify iteration number */
		iters = (long) (iters * factor);
	}

	printf ("     > Test Result: got %dus delay with %d iterations\n",
			(long) (delay / 1000ll), iters);
	printf ("       Stop after %d tests %s %dus threshold\n",
			test, llabs (desired_delay - delay) < threshold ?
				"reaching successfully" :
				"without success while reaching",
			(long) (threshold / 1000ll));

	printf ("\n ** Loop latency test... done\n");
}

void calibrate_timer_interrupt (long iters) {
	struct timespec tp, fi;
	int i;

	/* 1 second sleep */
	tp.tv_sec = 1;
	tp.tv_nsec = 0;

	for (i = 0; i < iters; i++) {
		/* sleep for 1 second */
		nanosleep (&tp, (struct timespec *) 0);

		/* display current time */
		clock_gettime (CLOCK_REALTIME, &fi);
		printf ("At iter. %03d; %ds, %dns\n", i, fi.tv_sec, fi.tv_nsec);
	}
}

void * test10_main(void *arg) {

	long long desired_delay = 375ll * 1000ll * 1000ll; /* 375 ms (in ns) */
	long long threshold = 1 * 1000 * 1000; /* 1 millisecond (in ns) */

	printf ("Test 10: ~~ Miscellaneous tests ~~\n");

	// calibrate_timer_interrupt (10 * 60);

	loop_lat (10000, desired_delay, threshold);

	clock_gettime_lat ();
	sleep_accuracy ();
	task_sleep ();

	printf ("Test 10: Done, bye!\n");
	return NULL;
}

