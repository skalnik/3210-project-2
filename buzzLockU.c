/* initialize a buzzlock lock with a maximum threshold of num_threads concurrent threads
 * with a gold priority elevation timeout period of timeout usecs.*/

#include "buzzLockU.h"

extern bzz_t;
extern useconds_t;
extern bzz_thread_t;
extern BZZ_BLACK;
extern BZZ_GOLD;


init_bzz( bzz_t *lock, int num_threads, useconds_t timeout){
	lock -> max_num_threads = num_threads;
	lock -> timeout = timeout;
	*(lock -> threads) = bzz_thread_t[num_threads];
}
/* the current thread has color black/gold with respect to lock lock */

bzz_color([BZZ_BLACK|BZZ_GOLD], bzz_t *lock){

}

/* Acquire the lock */

bzz_lock( bzz_t lock){

}

/* Release the lock */

bzz_release (bzz_t lock){

}

/* Destroy the lock */

bzz_kill(bzz_t lock){

}

