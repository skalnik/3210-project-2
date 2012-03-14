#include <linux/types.h>
#include <linux/spinlock.h>
#include <semaphore.h>
#include <linux/list.h>
#include <linux/time.h>
#include <linux/wait.h>

#ifndef BUZZLOCKK_H_
#define BUZZLOCKK_H_

#define BZZ_BLACK 1
#define BZZ_GOLD 0

typedef struct {
	struct list_head list;
	tid_t tid;
	int BZZ_COLOR;
	long queue_time; /*IN MICROSECONDS*/
}bzz_thread_t;

typedef struct {
	spinlock_t mutex = SPIN_LOCK_UNLOCKED;
	unsigned long flags;
	int num_active;
	tid_t locking_thread_tid;
	suseconds_t timeout;
	bzz_thread_t known_threads; /*never delete an entry, only add to tail*/
	struct wait_queue_head_t gold_queue;
	struct wait_queue_head_t black_queue;
	struct wait_queue_head_t old_gold_queue;
	int black_num;
	int gold_num;
	int old_gold_num;
}bzz_t;



#include <buzz.h>

#endif /* BUZZLOCKU_H_ */
