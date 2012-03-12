#include <stdlib.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <pthread.h>
#include "vendor/simclist.h"

#ifndef BUZZLOCKU_H_
#define BUZZLOCKU_H_

#define BZZ_BLACK 1
#define BZZ_GOLD  0

typedef struct {
	pid_t id;
	int color;
	double waiting_since;
} bzz_thread_t;

typedef struct {
	pthread_mutex_t mutex;
	pthread_cond_t  cond;
	int max_active_threads;
	int active_threads;
	useconds_t timeout;
	list_t *threads;
	list_t *waiting_gold_threads;
	list_t *waiting_black_threads;
} bzz_t;

#include "buzz.h"

void* get_thread(int, bzz_t);
int full_active_threads(bzz_t);
int is_black(bzz_thread_t*);
int is_gold(bzz_thread_t*);
int is_old(bzz_thread_t*, bzz_t);
void add_to_waiting_threads(bzz_thread_t*, bzz_t);
void wait(bzz_t);
double time_with_usec();
unsigned int num_black_waiting(bzz_t);
unsigned int num_gold_waiting(bzz_t);
unsigned int num_old_gold_waiting(bzz_t);
void add_active(bzz_thread_t*, bzz_t);
#endif
