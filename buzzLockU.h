#include <unistd.h>
#include "vendor/simclist.h"

#ifndef BUZZLOCK_H_
#define BUZZLOCK_H_

#define BZZ_BLACK 1
#define BZZ_GOLD  0

typedef struct {
	pid_t pid;
	int color;
} bzz_thread_t;

typedef struct {
	int locked;
	int max_active_threads;
	int num_gold_threads;
	int num_black_threads;
	int num_old_gold_threads;
	useconds_t timeout;
	bzz_thread_t *threads;
} bzz_t;

#endif