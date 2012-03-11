#include <linux/types.h>
#include <linux/spinlock.h>

#ifndef BUZZLOCKU_H_
#define BUZZLOCKU_H_

typedef int useconds_t;
typedef int BZZ_BLACK = 1;
typedef int BZZ_GOLD = 0;

typedef struct{
	tid_t TID;
	int BZZ_COLOR = -1;
}bzz_thread_t;

typedef struct {
	spinlock_t mutex = SPIN_LOCK_UNLOCKED;
	unsigned long flags;
	int locked = 0;
	int locking_thread_PID;
	int max_num_threads = 0;
	useconds_t timeout = 0;
	bzz_thread_t *threads;
}bzz_t;



#endif /* BUZZLOCKU_H_ */
