/* initialize a buzzlock lock with a maximum threshold of num_threads concurrent threads
 * with a gold priority elevation timeout period of timeout usecs.*/
#include "buzzLockK.h"
#include <linux/slab.h>
extern GFP_NOFAIL;


init_bzz(bzz_t *lock, int num_threads, suseconds_t timeout){
	lock -> timeout = timeout;
	lock ->num_active = num_threads;
	lock ->black_num = 0;
	lock ->gold_num = 0;
	lock ->old_gold_num = 0;
	INIT_LIST_HEAD(&lock -> known_threads.list);
	DECLARE_WAIT_QUEUE_HEAD(&lock->gold_queue);
	DECLARE_WAIT_QUEUE_HEAD(&lock->black_queue);
	DECLARE_WAIT_QUEUE_HEAD(&lock->old_gold_queue);
}

/* the current thread has color black/gold with respect to lock lock */
bzz_color(int color, bzz_t *lock){
	int i = 0;
	bzz_thread_t *temp, *tmp;
	struct list_head *pos;

	/*iterate through lock's thread list, find the calling thread's entry, set the color*/

	list_for_each(pos, &lock->known_threads.list){
		temp = list_entry(pos,&lock->known_threads.list, list);

		if (temp->tid == current->tgid){
			tmp = temp;
			temp->BZZ_COLOR = color;
			break;
		}

	}
	if (!tmp){/*thread doesnt have an entry, so we create and add it*/
		tmp = (struct bzz_thread_t *) kmalloc(sizeof(bzz_thread_t), GFP_NOFAIL);
		if (tmp){
			tmp ->BZZ_COLOR = color;
			tmp ->tid = current ->tgid;
			tmp ->queue_time = (current_kernel_time().tv_nsec)*1000; /*convert nano to micro seconds*/
			spin_lock_irqsave(&lock->mutex, lock->flags);

			list_add_tail(&tmp->list, &lock->known_threads.list);

			spin_lock_irqrestore(&lock->mutex, lock->flags);
		}
	}
	}
}

/* Acquire the lock */

bzz_lock(bzz_t *lock){
	/* check active #
	 * check if gold has been waiting too long
	 * if no black, queue gold
	 * queue black
	 * */
	 /*max active threads*/
	bzz_thread_t *temp, *tmp;
	struct list_head *pos, *loop;
	struct wait_queue_t wait;

	/*iterate through lock's thread list, find the calling thread's entry*/

	list_for_each(pos, &lock->known_threads.list){
		temp = list_entry(pos,&lock->known_threads.list, list);

		if (temp->tid == current->tgid){
			break;
		}
	}

	if(lock->num_active == 0){ /*all slots are taken*/
		init_waitqueue_entry(&wait, current);
		wait.flags |= WQ_FLAG_EXCLUSIVE;
		wait.func = wait.private->tgid == current->tgid;

		if(temp ->BZZ_COLOR == BZZ_BLACK){
			add_wait_queue(&lock->black_queue, &wait);
			lock->black_num++;
		}
		else{
			wait_event_timeout(&lock->gold_queue, wait.func, lock -> timeout);
			lock->gold_num++;
		}

		do{
			/*move timedout gold threads to old_gold_queue*/
			/*had issues with using the wait queue properly*/

			spin_lock_irqsave(&lock->mutex, lock->flags);
			if(!(lock->num_active == 0)){
				lock ->num_active--;
				spin_lock_irqrestore(&lock->mutex, lock->flags);

				if(temp -> BZZ_COLOR == BZZ_GOLD){
					if(is_old(temp, lock)){
						/*run old_gold*/
					}
					else if(){/*no black threads are waiting*/
						/*run gold*/
					}

				}
				else{ /*color is black*/
					/*run black*/
				}
			}
			spin_lock_irqrestore(&lock->mutex, lock->flags);
		}while(1);

	}
	else{ /*open thread slots*/

		spin_lock_irqsave(&lock->mutex, lock->flags);
		lock ->num_active--;
		spin_lock_irqrestore(&lock->mutex, lock->flags);

	}



}
/* Release the lock */

bzz_release (bzz_t *lock){
	spin_lock_irqsave(&lock->mutex, lock->flags);
	lock->num_active++;
	spin_lock_irqrestore(&lock->mutex, lock->flags);
	wake_up();
}

/* Destroy the lock */

bzz_kill(bzz_t lock){

}

int is_old(bzz_thread_t *thread, bzz_t *lock){
	return (lock->timeout) < ((current_kernel_time().tv_nsec * 1000) - thread -> queue_time );
}
