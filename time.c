#include <stdio.h>
#include <sys/time.h>
#include <sched.h>
#include <unistd.h>

#define ITS 2000000UL

unsigned long current_time() {
	struct timeval tv;
	gettimeofday(&tv,NULL);

	unsigned long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
	return time_in_micros;
}

unsigned long get_rr_time_quantum() {
	struct timespec t;
	pid_t pid = getpid();

	int ret = sched_rr_get_interval(pid, &t);
	return (unsigned long)((t.tv_sec * 1000000000ULL + t.tv_nsec) / 1000);
}

int main() {
	volatile unsigned long long i;
	unsigned long start, stop, diff;

	start = current_time();
	for (i = 0; i < ITS; ++i);
	stop = current_time();

	printf("Consumed: %lu (us)\n", stop - start);
	printf("Time quantum: %lu (us)\n", get_rr_time_quantum());
	return 0;
}
