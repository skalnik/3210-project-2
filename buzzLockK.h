#include <linux/types.h>
#include <linux/spinlock.h>
#include <list.h>

#ifndef BUZZLOCKK_H_
#define BUZZLOCKK_H_

typedef BZZ_BLACK = 1;
typedef BZZ_GOLD = 0;

typedef struct {
	struct list_head list;
	tid_t tid;
	int BZZ_COLOR;
	/*time stamp*/
}bzz_thread_t;

typedef struct {
	spinlock_t mutex = SPIN_LOCK_UNLOCKED;
	unsigned long flags;
	int num_locked;
	tid_t locking_thread_tid;
	suseconds_t timeout;
	bzz_thread_t known_threads;
	/*wait_threads queue(s)?*/
}bzz_t;

#include <buzz.h>

#endif /* BUZZLOCKU_H_ */
