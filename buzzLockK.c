/* initialize a buzzlock lock with a maximum threshold of num_threads concurrent threads
 * with a gold priority elevation timeout period of timeout usecs.*/
#include "buzzLockK.h"
#include <linux/slab.h>

extern GFP_ATOMIC;


init_bzz(bzz_t *lock, int num_threads, suseconds_t timeout){
	lock -> num_locked = num_threads;
	lock -> timeout = timeout;
	INIT_LIST_HEAD(&lock -> known_threads.list);
}

/* the current thread has color black/gold with respect to lock lock */
bzz_color(int color, bzz_t *lock){
	int i = 0;
	bzz_thread_t *temp, *tmp;
	struct list_head *pos;

	/*iterate through lock's thread list, find the calling thread's entry, set the color*/

	list_for_each(pos, &lock->known_threads.list){
		temp = list_entry(pos,&lock->known_threads.list);

		if (temp->tid == current->tgid){
			temp->BZZ_COLOR = color;
			break;
		}
		else{/*thread doesnt have an entry, so we create and add it*/
			tmp = (struct bzz_thread_t *) kmalloc(sizeof(bzz_thread_t), GFP_ATOMIC);
			if (tmp){
				spin_lock_irqsave(&lock->mutex, lock->flags);

				list_add(&tmp->list, &lock->known_threads.list);

				spin_lock_irqrestore(&lock->mutex, lock->flags);
			}
		}
	}
}

/* Acquire the lock */

bzz_lock(bzz_t *lock){
	spin_lock_irqsave(&lock->mutex, lock->flags);
	/* check active #
	 * check if gold has been waiting too long
	 * queue black
	 * if available queue gold
	 * */
	if (lock->num_locked == 0){ /*max active threads*/

	}
	else{ /*active thread slots*/

	}
//	if (lock -> num_locked == 0){
//		lock -> locked = 1;
//		lock -> locking_thread_PID = gettid();
//	}

	spin_lock_irqrestore(&lock->mutex, lock->flags);

}
/* Release the lock */

bzz_release (bzz_t *lock){
	spin_lock_irqsave(&lock->mutex, lock->flags);

//	if (lock -> locked == 1){
//		if(lock -> locking_thread_PID == gettid()){
//			lock -> locked = 0;
//		}
//	}
	spin_lock_irqrestore(&lock->mutex, lock->flags);
}

/* Destroy the lock */

bzz_kill(bzz_t lock){

}

