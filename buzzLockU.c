#include "buzzLockU.h"

void init_bzz(bzz_t *lock, int num_threads, useconds_t timeout) {
	lock->max_active_threads = num_threads;
	lock->timeout = timeout;
}
