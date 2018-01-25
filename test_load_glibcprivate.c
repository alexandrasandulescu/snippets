
#include <dlfcn.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <errno.h>
//#define _GNU_SOURCE

typedef int (*_sem_init_handler)(sem_t *sem, int pshared, unsigned int value);
typedef int (*_sem_timedwait_handler)(sem_t *sem, const struct timespec *abs_timeout);
typedef int (*_sem_wait_handler)(sem_t *sem);

_sem_init_handler _sem_init;
_sem_timedwait_handler _sem_timedwait;
_sem_wait_handler _sem_try_wait;
_sem_wait_handler _sem_wait;


int main() {
	void *handle = dlopen("/lib/i386-linux-gnu/libpthread.so.0", RTLD_LAZY);
	assert(handle != NULL);

	_sem_try_wait = dlsym(handle, "sem_trywait");
	_sem_init = dlsym(handle, "sem_init");
	assert(_sem_init != NULL);
	assert(_sem_try_wait != NULL);

	sem_t s;
	int ret = _sem_init(&s, 0, 0);
	assert(ret != -1);
	_sem_try_wait(&s);
	printf("%d\n", errno);


	return 0;
}
