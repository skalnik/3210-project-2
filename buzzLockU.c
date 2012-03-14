#include "buzzLockU.h"

void init_bzz(bzz_t *lock, int num_threads, useconds_t timeout) {
	lock->max_active_threads = num_threads;
	lock->timeout = timeout;
	pthread_mutex_init(&lock->mutex, NULL);
	pthread_cond_init(&lock->cond, NULL);
	// Malloc & init linked lists
	lock->threads = malloc(sizeof(list_t));
	list_init(lock->threads);
	lock->waiting_gold_threads = malloc(sizeof(list_t));
	list_init(lock->waiting_gold_threads);
	lock->waiting_black_threads = malloc(sizeof(list_t));
	list_init(lock->waiting_black_threads);
}

void bzz_kill(bzz_t *lock) {
	pthread_mutex_lock(&lock->mutex);
	// free & destroy linked lists
	list_destroy(lock->threads);
	list_destroy(lock->waiting_gold_threads);
	list_destroy(lock->waiting_black_threads);
	pthread_mutex_unlock(&lock->mutex);
	pthread_mutex_destroy(&lock->mutex);
	pthread_cond_destroy(&lock->cond);
}

pid_t get_thread_id() {
	return syscall(SYS_gettid);
}

void bzz_color(int color, bzz_t *lock) {
	pid_t id = get_thread_id();
	bzz_thread_t *found = (bzz_thread_t *)get_thread(id, lock);

	if(found == NULL) {
		// Not found create new thread & add to list of all threads
		bzz_thread_t *thread = malloc(sizeof(bzz_thread_t));
		thread->id = id;
		thread->color = color;
		list_append(lock->threads, thread);
	}
	else {
		// Found thread, just update color
		found->color = color;
	}
}

void bzz_lock(bzz_t *lock) {
	pid_t id = get_thread_id();
	pthread_mutex_lock(&lock->mutex);
	bzz_thread_t *thread = (bzz_thread_t *)get_thread(id, lock);
	
	if(full_active_threads(lock)) {
		// No active slots! Lets wait
		add_to_waiting_threads(thread, lock);

		do {
			pthread_cond_wait(&lock->cond, &lock->mutex);

			// Woken up, see if we can become active

			if(!full_active_threads(lock)) {
				if(is_gold(thread)) {
					if(is_old(thread, lock)) {
						add_active(thread, lock);
						pthread_mutex_unlock(&lock->mutex);
						return;
					}
				  else if(num_black_waiting(lock) <= 0) {
						add_active(thread, lock);
						pthread_mutex_unlock(&lock->mutex);
						return;
					}
				}
				else {
					if(num_old_gold_waiting(lock) <= 0) {
						add_active(thread, lock);
						pthread_mutex_unlock(&lock->mutex);
						return;
					}
				}
			}
		} while(1); // Loop again due to accidental wake ups
	}
	else {
		// Hey a slot just for us as we were created!
		add_active(thread, lock);
		pthread_mutex_unlock(&lock->mutex);
		return;
	}
}

void bzz_release(bzz_t *lock) {
	pthread_mutex_lock(&lock->mutex);
	lock->active_threads--;
	pthread_mutex_unlock(&lock->mutex);
	pthread_cond_broadcast(&lock->cond);
}

/* NOT THREAD SAFE! */
/* ONLY CALL WHEN MUTEX ACQUIRED */
void add_active(bzz_thread_t *thread, bzz_t *lock) {
	// Determine thread color and remove from correct waiting list
	if(is_gold(thread)) {
		int position = list_locate(lock->waiting_gold_threads, thread);
		list_delete_at(lock->waiting_gold_threads, position);
	} else {
		int position = list_locate(lock->waiting_black_threads, thread);
		list_delete_at(lock->waiting_black_threads, position);
	}
	lock->active_threads++;
}

int is_old(bzz_thread_t *thread, bzz_t *lock) {
	// Adapted from test.c
	useconds_t elapsed_usec = (useconds_t)((thread->waiting_since - time_with_usec()) * 1000000.0);
	return elapsed_usec > lock->timeout;
}

int is_black(bzz_thread_t *thread) {
	return thread->color == BZZ_BLACK;
}

int is_gold(bzz_thread_t *thread) {
	return thread->color == BZZ_GOLD;
}

void add_to_waiting_threads(bzz_thread_t *thread, bzz_t *lock) {
	// Set waiting time & then append to correct list
	thread->waiting_since = time_with_usec();
	if(is_gold(thread)) {
		list_append(lock->waiting_gold_threads, thread);
	} else {
		list_append(lock->waiting_black_threads, thread);
	}
}

double time_with_usec() {
	struct timeval time;
	return time.tv_sec + (time.tv_usec / 1000000.0);
}

unsigned int num_black_waiting(bzz_t *lock) {
	unsigned int ret = list_size(lock->waiting_black_threads);
	return ret;
}

unsigned int num_gold_waiting(bzz_t *lock) {
	unsigned int ret = list_size(lock->waiting_gold_threads);
	return ret;
}

int full_active_threads(bzz_t *lock) {
	return lock->active_threads >= lock->max_active_threads;
}

unsigned int num_old_gold_waiting(bzz_t *lock) {
	unsigned int num_old_gold_waiting = 0;
	list_t *waiting_gold_threads = lock->waiting_gold_threads;

	list_iterator_start(waiting_gold_threads);
	// Iterate through waiting gold threads checking for old threads
	while(list_iterator_hasnext(waiting_gold_threads)) {
		bzz_thread_t *current = list_iterator_next(waiting_gold_threads);
		if(is_old(current, lock)) {
			num_old_gold_waiting++;
		}
	}
	list_iterator_stop(waiting_gold_threads);

	return num_old_gold_waiting;
}

void* get_thread(int id, bzz_t *lock) {
	bzz_thread_t *found = 0;

	list_t *threads = lock->threads;
	list_iterator_start(threads);

	while(list_iterator_hasnext(threads)) {
		bzz_thread_t *current = list_iterator_next(threads);
		if(current->id == id) {
			found = current;
		}
	}
	list_iterator_stop(threads);

	return found;
}

