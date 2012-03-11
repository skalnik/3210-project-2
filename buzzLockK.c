/* initialize a buzzlock lock with a maximum threshold of num_threads concurrent threads
 * with a gold priority elevation timeout period of timeout usecs.*/

#include "buzzLockU.h"

extern struct bzz_t;
extern useconds_t;
extern struct bzz_thread_t;
extern BZZ_BLACK;
extern BZZ_GOLD;


init_bzz(struct bzz_t *lock, int num_threads, useconds_t timeout){
	lock -> max_num_threads = num_threads;
	lock -> timeout = timeout;
	*(lock ->threads) = bzz_thread_t[num_threads];
}
/* the current thread has color black/gold with respect to lock lock */

bzz_color(int color, bzz_t *lock){
	spin_lock_irqsave(&lock->mutex, lock->flags);

	int i = 0;
	struct bzz_thread_t *temp;
	/*iterate through lock's thread list, find the calling thread's entry, set the color*/
	for(;i<&lock->max_num_threads;i++){
		*temp = &lock -> threads[i];
		if (temp->TID == gettid()){
			temp->BZZ_COLOR = color;
		}
	}

	spin_lock_irqrestore(&lock->mutex, lock->flags);
}

/* Acquire the lock */

bzz_lock(struct bzz_t *lock){
	spin_lock_irqsave(&lock->mutex, lock->flags);

	if (lock -> locked == 0){
		lock -> locked = 1;
		lock -> locking_thread_PID = gettid();
	}

	spin_lock_irqrestore(&lock->mutex, lock->flags);

}
/* Release the lock */

bzz_release (struct bzz_t *lock){
	spin_lock_irqsave(&lock->mutex, lock->flags);

	if (lock -> locked == 1){
		if(lock -> locking_thread_PID == gettid()){
			lock -> locked = 0;
		}
	}
	spin_lock_irqrestore(&lock->mutex, lock->flags);
}

/* Destroy the lock */

bzz_kill(bzz_t lock){

}

